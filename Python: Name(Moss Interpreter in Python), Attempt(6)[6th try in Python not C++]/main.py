import shell

lexed_list = []
parsed_list = []
shell.global_symbol_table = shell.SymbolTable()
while True:
    line = input(">>>")
    if line == "quit":
        print("Program execution terminated with return code 0")
        exit(0)
    elif line == "lex":
        print(list(lexed_list))
    elif line == "parse":
        print(list(parsed_list.items))
    else:
        lex = shell.Lexer(line)
        res = lex.lex()
        if isinstance(res, int) == False and res[1] != shell.ERROR:
            parse = shell.Parser(res[0])
            res2 = parse.parse()
            lexed_list = res[0]
            if isinstance(res2, int) == False and res2[1] != shell.ERROR:
                executor = shell.Executor(res2[0]).execute()
                if executor != shell.ERROR:
                    print(executor.value)
                else:
                    print("Program execution terminated... with return code -1")
                    exit(-1)
            else:
                print("Program execution terminated... with return code -1")
                exit(-1)
        else:
            print("Program execution terminated... with return code -1")
            exit(-1)