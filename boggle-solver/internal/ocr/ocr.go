package ocr

import (
	"github.com/otiai10/gosseract/v2"
)

func GetText(path string) (string, error) {
	client := gosseract.NewClient()
	defer client.Close()
	client.SetImage(path)
	client.SetPageSegMode(gosseract.PSM_SINGLE_CHAR)
	client.SetWhitelist("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
	return client.Text()
}
