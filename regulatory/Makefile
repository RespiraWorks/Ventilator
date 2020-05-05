SHELL := /bin/bash -o pipefail -e
.SUFFIXES:
.SECONDARY:
.DELETE_ON_ERROR:

SYSTEM_CONFIG := data/system.yml
DATA_FILES := $(wildcard data/*.yml)
TEMPLATES := $(wildcard templates/*.md)
RELEASE_DOCUMENTS := $(patsubst documents/%.md,release/%.md,$(wildcard documents/*.md))
RELEASE_PDFS := $(patsubst documents/%.md,release/%.pdf,$(wildcard documents/*.md))
RELEASE_JSONS := $(patsubst documents/%.md,release/%.pdf,$(wildcard documents/*.md))

all: $(RELEASE_DOCUMENTS)

pdfs: $(RELEASE_PDFS)

jsons: $(RELEASE_JSONS)

.PHONY:
github_issues: $(SYSTEM_CONFIG)
	rdm pull $<

release/%.md: documents/%.md $(DATA_FILES) $(TEMPLATES)
	@mkdir -p $(@D)
	rdm render $< $(DATA_FILES) > $@

tmp/%.tex: release/%.md
	@mkdir -p $(@D)
	rdm tex $< $(DATA_FILES) > $@

release/%.pdf: tmp/%.pdf
	cp $< $@

tmp/%.pdf: tmp/%.tex
	latexmk -pdf -jobname="./$(basename $@)" $<

.PHONY:
clean:
	rm -rf release/**.pdf tmp
