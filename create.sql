PRAGMA encoding = 'UTF-8';
CREATE VIRTUAL TABLE pages using fts5(url, title, body)
