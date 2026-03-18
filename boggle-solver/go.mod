module boggle-solver

go 1.24.3

replace internal/ocr => ./internal/ocr

replace internal/solver => ./internal/solver

require internal/ocr v0.0.0-00010101000000-000000000000

require github.com/otiai10/gosseract/v2 v2.4.1 // indirect
