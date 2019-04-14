#coding: utf-8
import htmlmin
import csscompressor

with open("index.html", "r") as f:
	src_html = f.read()

with open("app.js", "r") as f:
	src_js = f.read()

with open("404.html", "r") as f:
	notfound_html = f.read()
	notfound_html = htmlmin.minify(notfound_html, remove_empty_space=True, remove_comments=True)
	notfound_html = notfound_html.replace("\"", "'")
	notfound_html = notfound_html.replace("\n", "")
	notfound_html = notfound_html.replace("\t", "")

with open("saved.html", "r") as f:
	saved_html = f.read()
	saved_html = htmlmin.minify(saved_html, remove_empty_space=True, remove_comments=True)
	saved_html = saved_html.replace("\"", "'")
	saved_html = saved_html.replace("\n", "")
	saved_html = saved_html.replace("\t", "")

with open("style.css", "r") as f:
	src_css = csscompressor.compress(f.read())

src_html = src_html.replace("""<link rel="stylesheet" href="style.css">""", "<style>{}</style>".format(src_css))
src_html = src_html.replace("""<script src="app.js"></script>""", "<script>{}</script>".format(src_js))
src_html = htmlmin.minify(src_html, remove_empty_space=True, remove_comments=True)
src_html = src_html.replace("\"", "'")
src_html = src_html.replace("\n", "")
src_html = src_html.replace("\t", "")

with open("../train/WebUITemplates.h", "w", encoding="utf-8") as f:
	f.write("#define PAGE_CONTENT_INDEX \"")
	f.write(src_html)
	f.write("\"\n")
		
	f.write("#define PAGE_CONTENT_SAVED \"")
	f.write(saved_html)
	f.write("\"\n")
		
	f.write("#define PAGE_CONTENT_404 \"")
	f.write(notfound_html)
	f.write("\"")
