all:
	@gcc src/lexer.c -o build/waffle

run:
	@build/waffle
