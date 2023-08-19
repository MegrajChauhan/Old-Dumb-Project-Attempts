from curses.ascii import isalpha
import os
from typing import List

MAIN = "main"
NUM = "num"
DEC = "dec"
FUNC = "func"

LPAREN = "("
RPAREN = ")"
LCURLY = "{"
RCURLY = "}"
EQ = "="
EOL = ";"
COMMA = ","

num = '0123456789'

kw = [MAIN, NUM, DEC, FUNC]
opers = [LPAREN, RPAREN, LCURLY, RCURLY, EQ, EOL, COMMA]

class Tokens:
    def __init__(self, type, value = "") -> None:
        self.type = type
        self.value = value

    def __repr__(self) -> str:
        return f'{self.type}:{self.value if self.value != "" else self.value}'

def is_identifier(check):
    invalid_count = 0
    for x in check:
        if x == '_' or isalpha(x) == True:
            continue
        else:
            invalid_count += 1
    if invalid_count > 0:
        return False
    return True

def num_type(check):
    dot_count = 0
    valid =  0
    for a in check:
        if a in num:
            valid += 1
        elif a == '.':
            valid+=1
            dot_count += 1
    if dot_count == 1 and valid == len(check):
        return Tokens("dec", check)
    elif dot_count == 0 and valid == len(check):
        return Tokens("num", check)
    else:
        return Tokens("ERROR", check)

def iden(check):
    if check in kw:
        return Tokens(check)
    elif is_identifier(check) == True:
        return Tokens("ID", check)
    elif check in opers:
        if check == LPAREN:
            return Tokens("LPAREN")
        elif check == RPAREN:
            return Tokens("RPAREN")
        elif check == RCURLY:
            return Tokens("RCURLY")
        elif check == LCURLY:
            return Tokens("LCURLY")
        elif check == EQ:
            return Tokens("EQ")
        elif check == EOL:
            return Tokens("EOL")
        elif check == COMMA:
            return Tokens("COMMA")
    else:
            return Tokens("Error", check)
        


# error
class Error:
    def __init__(self, file_name, line, column, type, level) -> None:
        self.file = file_name
        self.line = line
        self.column = column
        self.type = type
        self.level = level

    def __repr__(self) -> str:
        return f'{self.file}:{self.level}'


err_list = []

# File Reader


class FileReader:
    def __init__(self, file_name) -> None:
        self.file_name = file_name

    def exists(self):
        if (os.path.isfile(self.file_name)):
            return True
        else:
            return False

    def is_valid(self):
        if (self.file_name[len(self.file_name) - 1] == 't'):
            if (self.file_name[len(self.file_name) - 2] == 'p'):
                if (self.file_name[len(self.file_name) - 3] == 'j'):
                    if (self.file_name[len(self.file_name) - 4] == '.'):
                        return True
        return False

    def readFile(self):
        if (self.is_valid() == True):
            if (self.exists() == True):
                with open(self.file_name) as r:
                    return r.read()
            else:
                err_list.append(Error(self.file_name, 0, 0,
                                "File Doesn't exists", "Fatal"))
        else:
            err_list.append(Error(self.file_name, 0, 0,
                            "Unsupported File Format", "Fatal"))

# lexer


class Lexer:
    def __init__(self, file_name) -> None:
        self.file_name = file_name
        self.contents = FileReader(file_name).readFile()
        self.ind = 0
        self.cur = self.contents[0]
        self.tok_list = []
        self.has_main = False

    def advance(self):
        self.ind += 1
        self.cur = self.contents[self.ind] if self.ind < len(self.contents) else None

    def lex(self):
        while self.cur != None:
            if self.cur in ' \n\0\t':
                self.advance()
            elif isalpha(self.cur) == True:
                xyz = self.cur
                self.advance()
                while isalpha(self.cur) == True:
                    xyz += self.cur
                    self.advance()
                self.tok_list.append(iden(xyz))
                if xyz == "main":
                    self.has_main = True
            elif self.cur in opers:
                self.tok_list.append(iden(self.cur))
                self.advance()
            elif self.cur in num:
                numb = self.cur
                self.advance()
                while self.cur in num or self.cur == '.':
                    numb += self.cur
                    self.advance()
                self.tok_list.append(num_type(numb))
        # if self.has_main == False:
        #     print("No entry function defined!")


