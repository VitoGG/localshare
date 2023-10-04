#!/usr/bin/env python3
import cgi
import os

def main():
    # Create a form object to access uploaded file data
    form = cgi.FieldStorage()

    # Set the content type to HTML
    print("Content-type: text/html\n")
    print("<head><title>Arquivo Salvo</title>")
    print("<link rel='stylesheet' href='../assets/css/bootstrap.min.css' integrity='...'' crossorigin='anonymous' />")
    print("<script src='../assets/js/react.development.js'></script>")
    print("<script src='../assets/js/react-dom.development.js'></script>")
    print("<script src='../assets/js/react-dom.development.js'></script>")

    # Check if the 'file' field exists in the form
    if "file" in form:
        # Get the uploaded file object
        file_item = form["file"]

        # Check if the file was successfully uploaded
        if file_item.filename:
            handle_uploaded_file(file_item)
        else:
            print("No file was selected for upload.")
    else:
        print("File upload field is missing in the form.")

def handle_uploaded_file(file_item):
    # Specify the directory to save the uploaded files (e.g., "uploads")
    upload_dir = "uploads"

    # Create the upload directory if it doesn't exist
    if not os.path.exists(upload_dir):
        os.makedirs(upload_dir)

    # Get the file name
    filename = os.path.join(upload_dir, os.path.basename(file_item.filename))

    # Save the file to the server
    with open(filename, "wb") as f:
        f.write(file_item.file.read())

    print(f"<div class='alert alert-success m-5' role='alert'>File '{filename}' has been uploaded successfully.</div>")

if __name__ == "__main__":
    main()