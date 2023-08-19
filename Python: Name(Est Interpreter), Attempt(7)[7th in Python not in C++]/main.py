import shell

def main():
   xop = shell.Lexer(input("Enter file name: "))
   xop.lex()

main()