#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// Função para encontrar o delimitador que marca o fim do conteúdo do arquivo
char *find_end_of_content(const char *buffer, size_t length) {
    const char *end_of_content = strstr(buffer, "\r\n------WebKitFormBoundary");
    if (end_of_content != NULL) {
        return (char *)end_of_content;
    }
    return NULL;
}

char *find_start_of_content(const char *buffer, size_t length) {
    const char *start_of_content = strstr(buffer, "\r\n\r\n");;
    if (start_of_content != NULL) {
        return (char *)start_of_content;
    }
    return NULL;
}

int main() {
    char *content_length_str = getenv("CONTENT_LENGTH");
    long content_length = content_length_str ? strtol(content_length_str, NULL, 10) : 0;

    printf("Content-type: text/html\r\n\r\n");
    printf("<head><title>File Uploaded</title>");
    printf("<link rel='stylesheet' href='../assets/css/style.css' integrity='...' crossorigin='anonymous' />");


    if (content_length > 0) {
        char *buffer = malloc(content_length);
        if (buffer == NULL) {
            printf("Error: Unable to allocate memory.\n");
            return 1;
        }

        ssize_t bytes_read = fread(buffer, 1, content_length, stdin);
        if (bytes_read == -1) {
            printf("Error reading input.\n");
            free(buffer);
            return 1;
        }

        // Encontrar o delimitador que marca o fim do conteúdo do arquivo
        char *end_of_content = find_end_of_content(buffer, bytes_read);
        char *start_of_content = find_start_of_content(buffer, bytes_read);    
        char *sum = start_of_content - end_of_content;
        if (end_of_content != NULL) {
            start_of_content += 4;  
            // Calcule o tamanho do conteúdo do arquivo
            size_t content_length =  bytes_read -  (sum - buffer);

            // Verifique se o tamanho do conteúdo é válido
            if (content_length > 0) {
                // Nome do arquivo pode ser gerado ou obtido de outra forma
                char filename[] = "file.txt";

                // Diretório de upload
                const char *upload_dir = "uploads";
                if (access(upload_dir, F_OK) == -1) {
                    mkdir(upload_dir, 0777);
                }

                char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", upload_dir, filename);

                FILE *output_file = fopen(full_path, "wb");
                if (output_file != NULL) {
                    fwrite(buffer, 1, content_length, output_file);
                    fclose(output_file);
                    printf("File '%s' has been uploaded successfully.\n", full_path);
                } else {
                    printf("Error: Unable to open file for writing.\n");
                }
            } else {
                printf("Error: No content in the uploaded file.\n");
            }
        } else {
            printf("Error: Unable to determine the end of file content.\n");
        }

        free(buffer);
    } else {
        printf("No file was selected for upload.\n");
    }

    return 0;
}
