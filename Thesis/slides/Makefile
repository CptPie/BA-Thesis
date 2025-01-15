# define the tex sources to be used
SOURCES = slides.tex
# define depenencies for the docker build
DOCKERSOURCES = Dockerfile deps.txt

# define the options for the lualatex compiler
OPTS = -shell-escape -interaction=nonstopmode

# this is the main target, it will compile the pdf
slides.pdf: $(SOURCES)
	lualatex $(OPTS) slides.tex

present: slides.pdf
	WEBKIT_DISABLE_DMABUF_RENDERER=1 pdfpc slides.pdf

# this target will compile the pdf in a docker container, the results will be moved to the main directory
docker: $(SOURCES) $(DOCKERSOURCES)
	# build the image
	docker build . -t build
	# run the image as container, autoremoves the container after the run
	docker run --rm -v "${PWD}:/build" --name build build
	# remove the image (will not effect cached partial images)
	docker rmi build

# remove all the files that are created during the compilation
clean:
	latexmk -c && rm -f main.pdf && rm -f main.bbl && rm -f main.run.xml && rm -rf _minted-main

# move the pdf to the pdf directory
move:
	if [ -d pdf ]; then \
		mv *.pdf pdf/.; \
	else \
		mkdir pdf; \
		mv *.pdf pdf/.; \
	fi
