find . -type f -name "./*.h" -not -path "./external/*" -not -path "./bin/*"  | xargs ctags -x --c-kinds=f
