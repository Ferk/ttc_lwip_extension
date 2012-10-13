#
# Makefile for generating the LaTeX docs
#
# Fernando Carmona Varo 2012


PDF=example_lwip.pdf
#PDF=eth_soundcard_doc.pdf

OUTDIR=./tmp

TEX=lualatex
TOPTS= --output-directory=$(OUTDIR)


all: clean $(PDF)

%.pdf: %.tex
	@ echo "** Generating $(@)"
	mkdir -p $(OUTDIR) $(OUTDIR)/chapters
	$(TEX) $(TOPTS) $^
	mv $(OUTDIR)/$@ .

clean:
	#rm -rf $(OUTDIR) $(PDF)
	rm -rf $(PDF)


