
.global init
.text

init:
	li 1, 1
	li 2, 2
	li 3, 3
	li 4, 4
	li 5, 5

loop:
	b loop
