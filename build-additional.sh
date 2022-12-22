mkdir -p build
html-minifier --collapse-whitespace --remove-comments \
--remove-optional-tags --remove-redundant-attributes --remove-script-type-attributes \
--remove-tag-whitespace --use-short-doctype --minify-css true \
--minify-js true web/index.html > build/index.html
 groovy build-web.groovy