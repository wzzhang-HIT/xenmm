#!/bin/sh

# rewrite eps file to compatible with word

epstopdf $1 -o /tmp/$1.pdf
pdftops -level2 -eps /tmp/$1.pdf $1
