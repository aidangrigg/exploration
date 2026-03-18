package main

import (
	"html/template"
	"io"
	"log"
	"net/http"
	"os"
	"strings"
	"internal/ocr"
)

const (
	OS_READ        = 04
	OS_WRITE       = 02
	OS_EX          = 01
	OS_USER_SHIFT  = 6
	OS_GROUP_SHIFT = 3
	OS_OTH_SHIFT   = 0

	OS_USER_R   = OS_READ << OS_USER_SHIFT
	OS_USER_W   = OS_WRITE << OS_USER_SHIFT
	OS_USER_X   = OS_EX << OS_USER_SHIFT
	OS_USER_RW  = OS_USER_R | OS_USER_W
	OS_USER_RWX = OS_USER_RW | OS_USER_X

	OS_GROUP_R   = OS_READ << OS_GROUP_SHIFT
	OS_GROUP_W   = OS_WRITE << OS_GROUP_SHIFT
	OS_GROUP_X   = OS_EX << OS_GROUP_SHIFT
	OS_GROUP_RW  = OS_GROUP_R | OS_GROUP_W
	OS_GROUP_RWX = OS_GROUP_RW | OS_GROUP_X

	OS_OTH_R   = OS_READ << OS_OTH_SHIFT
	OS_OTH_W   = OS_WRITE << OS_OTH_SHIFT
	OS_OTH_X   = OS_EX << OS_OTH_SHIFT
	OS_OTH_RW  = OS_OTH_R | OS_OTH_W
	OS_OTH_RWX = OS_OTH_RW | OS_OTH_X

	OS_ALL_R   = OS_USER_R | OS_GROUP_R | OS_OTH_R
	OS_ALL_W   = OS_USER_W | OS_GROUP_W | OS_OTH_W
	OS_ALL_X   = OS_USER_X | OS_GROUP_X | OS_OTH_X
	OS_ALL_RW  = OS_ALL_R | OS_ALL_W
	OS_ALL_RWX = OS_ALL_RW | OS_GROUP_X
)

func indexHandler(w http.ResponseWriter, r *http.Request) {
	renderTemplate(w, "index")
}

func uploadHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Redirect(w, r, "/", http.StatusSeeOther)
	}

	recievedFile, header, err := r.FormFile("file")
	if err != nil {
		log.Printf("An error occured: %s", err)
		http.Error(w, "Failed to read the file", http.StatusInternalServerError)
		return
	}
	defer recievedFile.Close()

	exts := strings.Split(header.Filename, ".")
	ext := ".png" // default to png
	if len(exts) >= 2 {
		ext = exts[len(exts) - 1]
	}

	filepath := "./uploads/file." + ext

	serverFile, err := os.OpenFile(filepath, os.O_WRONLY|os.O_CREATE, OS_USER_RW)
	if err != nil {
		log.Printf("An error occured: %s", err)
		http.Error(w, "Could not create file", http.StatusInternalServerError)
		return
	}
	defer serverFile.Close()

	_, err = io.Copy(serverFile, recievedFile)
	if err != nil {
		log.Printf("An error occured: %s", err)
		http.Error(w, "Could not save to file", http.StatusInternalServerError)
		return
	}

	text, err := ocr.GetText(filepath)
	if err != nil {
		log.Printf("An error occured: %s", err)
		http.Error(w, "Could not extract text from image", http.StatusInternalServerError)
		return
	}

	log.Printf("Text extracted: \"%s\"", text)
	http.Redirect(w, r, "/", http.StatusFound)
}

func solveHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Redirect(w, r, "/", http.StatusSeeOther)
	}

	r.ParseForm()
	for key, val := range r.Form {
		log.Printf("index: %s, value: %s", key, val)
	}
	http.Redirect(w, r, "/", http.StatusFound)
}

var templates = template.Must(template.ParseFiles("static/index.html"))

func renderTemplate(w http.ResponseWriter, tmpl string) {
	err := templates.ExecuteTemplate(w, tmpl+".html", nil)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func main() {
	http.HandleFunc("/", indexHandler)
	http.HandleFunc("/upload", uploadHandler)
	http.HandleFunc("/solve", solveHandler)

	host, port := "0.0.0.0", ":8080"
	log.Printf("Starting server on %s%s", host, port)
	log.Fatal(http.ListenAndServe(host + port, nil))
}
