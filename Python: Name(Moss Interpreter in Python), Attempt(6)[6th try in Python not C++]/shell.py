import string

global_file_name = "<stdin>"  # default file name
line_no = 1  # default line number
col_no = 1  # default column number
global_line = ""

num_constants = "0123456789"
alpha = string.ascii_letters
opers = "+-/*^%~|=&({[]}<>,.\"\')!:;"

ERROR = -1  # returned by functions if anything unexpected happens
SUCCESS = 1  # returned by functions and classes upon the success of process

# error types for error class
UNKNOWN_TOKEN = 1
INCORRECT_NUMBER = 2
UNKNOWN_OPER = 3
EOF_REACHED = 4
STRAY_PAREN = 5
EXPECTED_NUMBER = 6
CHAR_LIMIT = 7
CONCAT_ONLY_STR = 8
DIV_BY_ZERO = 9
EOS_NEEDED = 10
VAR_OUT_OF_SCOPE_OR_NOT_EXIST = 11
REDECLARATION = 12
MISMATCHED_VALUES = 13

#Scopes
GLOBAL = "GLOBAL"
LOCAL = "LOCAL"

# operators
PLUS_TOKEN = "PLUS"
MINUS_TOKEN = "MINUS"
MULTIPLY_TOKEN = "MULTIPLY"
DIVIDE_TOKEN = "DIVIDE"
POW_TOKEN = "POWER"
MOD_TOK = "MOD"
EQUAL = "EQUAL"
GREATER_TOKEN = "GREATER_THAN"
SMALLER_TOKEN = "SMALLER_THAN"
OPAREN = "OPAREN"
CPAREN = "CPAREN"
OCURLY = "OCURLY"
CCURLY = "CCURLY"
OBIG = "OBIGBRAC"
CBIG = "CBIGBRAC"
LOGICAL_OR = "OR"
LOGICAL_AND = "AND"
LOGICAL_IS_EQUAL_TO = "IS_EQUAL"
LOGICAL_IS_NOT_EQUAL_TO = "IS_NOT_EQUAL"
LOGICAL_NOT = "NOT"
COMMENT = "COMMENT"
DOT_DECIMAL = "POINT"
DOUBLE_QUOTE = "D_QUOTE"
SINGLE_QUOTE = "S_QUOTE"
COMMA = "COMMA"
PLUS_EQUAL = "PLUS_EQ"
MINUS_EQUAL = "MINUS_EQ"
MUL_EQUAL = "MUL_EQ"
DIV_EQUAL = "DIV_EQ"
MOD_EQUAL = "MOD_EQ"
INC = "INC"
DEC = "DECI"
EOS = "EOS"
COLON = "COLON"
MULTI_LINE_STR = "MULTI_LINE_STR"

# keywords
NUM = "NUM"
DECI = "DEC"
BOOL = "BOOL"
STRING = "STRING"
CHAR = "CHAR"
BOOL_TRUE = "TRUE"
BOOL_FALSE = "FALSE"
FUNCTION = "FUNCTION"
FOR = "FOR"
WHILE = "WHILE"
DO = "DO"
KEY_OR = "K_OR"
KEY_AND = "K_AND"
RETURN = "RETURN"
IF = "IF"
ELSE = "ELSE"

IDENTIFIER = "ID"
STRING_VAL = "STRING_VAL"
CHAR_VAL = "CHAR_VAL"

types = [NUM, DECI,BOOL,STRING, CHAR]

toks_list_oper = {PLUS_TOKEN: "+", MINUS_TOKEN: "-", MULTIPLY_TOKEN: "*", DIVIDE_TOKEN: "/", POW_TOKEN: "^", MOD_TOK: "%", EQUAL: "=", GREATER_TOKEN: ">", SMALLER_TOKEN: "<",
    OPAREN: "(", CPAREN: ")", OCURLY: "{", CCURLY: "}", OBIG: "[", CBIG: "]", LOGICAL_OR: "||", LOGICAL_AND: "&&", LOGICAL_NOT:"!", LOGICAL_IS_EQUAL_TO: "==", COMMENT: "~", DOT_DECIMAL: ".", DOUBLE_QUOTE: "\"", SINGLE_QUOTE: "\'", COMMA: ",", PLUS_EQUAL: "+=", MINUS_EQUAL: "-=", MUL_EQUAL: "*=", DIV_EQUAL: "/=", MOD_EQUAL: "%=", INC: "++", DEC: "--", EOS:";", COLON:":", MULTI_LINE_STR:"\'\'"}

toks_list_keywords = {NUM: "num", DECI: "dec", BOOL: "bool", STRING: "string", CHAR: "char", BOOL_TRUE: "true",
                      BOOL_FALSE: "false", FUNCTION: "func", FOR: "for", WHILE: "while", DO: "do", KEY_AND: "and", KEY_OR: "or", RETURN: "return", IF:"if", ELSE:"else"}


#node types
EXPRESSION_NODE = "EXPRESSION"
VAR_DECLR = "VARIABLE DECLARATION"

def get_oper_type(check_for):
    for key, item in toks_list_oper.items():
        if item == check_for:
            return key
    return ERROR


def get_key_type(check_for):
    for key, item in toks_list_keywords.items():
        if item == check_for:
            return key
    return ERROR


def get_num_type(check_for):
    dot_count = 0
    count = 0
    type = NUM
    for x in check_for:
        if x in num_constants:
            count += 1
        elif x == '.':
            dot_count += 1
            count += 1
            type = DECI
        if check_for[0] == '-':
            count+=1
    if count < len(check_for) and dot_count > 1: return ERROR
    return type


# so the grammer allows for identifiers like "_______" also
def is_identifier_valid(check_for):
    count = 0
    for x in check_for:
        if x in alpha:
            count += 1
        elif x == "_":
            count += 1
    if count == len(check_for): return True
    else: return ERROR

def operator_precedence(oper):
    if oper in "+-":
        return 3
    elif oper in "/*%":
        return 4
    elif oper in "^":
        return 5
    elif oper == ';-':
        return 6
    elif oper == "&&" or oper == "and":
        return 1
    elif oper == "||" or oper == "or":
        return 2
    else:
        return 0

def operator_associativity(op):
    if op in ["**"]:
        return "right"  # right-associative
    elif op in ["+", "-", "*", "/", "%", "//", "<<", ">>", "&", "|", "^"]:
        return "left"  # left-associative
    else:
        return 0  # non-associative
# General error viewing:
# FILENAME: [LEVEL OF ERROR]: ERRORNUMBER: LINE: COLUMN: In line "error line"
#                                                          print"^~~~~~" from error column to the end of line
class Error:
    def __init__(self, line_num, col_num, type, err_line) -> None:
        self.line_num = line_num
        self.col_no = col_num
        self.file_name = global_file_name
        self.type = type
        self.line = err_line
        self._call_error()

    def _call_error(self):
        if self.type == UNKNOWN_TOKEN:
            self.unknown_tok()
        elif self.type == INCORRECT_NUMBER:
            self.incorrect_number()
        elif self.type == UNKNOWN_OPER:
            self.unknown_operator()
        elif self.type == EOF_REACHED:
             self.unexpected_eof()
        elif self.type == STRAY_PAREN:
             self.stray_paren()
        elif self.type == EXPECTED_NUMBER:
            self.expected_number()
        elif self.type == CHAR_LIMIT:
            self.char_limit()
        elif self.type == CONCAT_ONLY_STR:
            self.concat_str()
        elif self.type == EOS_NEEDED:
            self.semi_colon()
        elif self.type == VAR_OUT_OF_SCOPE_OR_NOT_EXIST:
            self.var_not_reached()
        elif self.type == REDECLARATION:
            self.redeclaration()
        elif self.type == MISMATCHED_VALUES:
            self.mismatched_values()

    # prints the token that caused the error
    def unknown_tok(self):
        print(self.file_name + ":" + "FATAL" + ":100:" +str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\": Unknown token type expected num, dec, or function")
        print(self.line)
        x = 0
        strx = ""
        while x <= self.col_no - 1:
            strx+=" "
            x+=1
        strx+= "^"
        while x <= len(self.line):
            strx+="~"
            x+=1
        print(strx)

    def incorrect_number(self):
         print(self.file_name + ":" + "FATAL" + ":101:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\": Wrong number format: expected a num or dec instead")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

    def unknown_operator(self):
         print(self.file_name + ":" + "FATAL" + ":102:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\": Unknown Operator: expected +, - , *, / or (,),{,[,],} etc")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)
        
    def unexpected_eof(self):
         print(self.file_name + ":" + "FATAL" + ":103:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\":Unexpected EOL reached +, - , *, / or (,),{,[,],} etc")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

    def stray_paren(self):
         print(self.file_name + ":" + "FATAL" + ":104:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\":Expected a closing parenthesis here:")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

    def expected_number(self):
         print(self.file_name + ":" + "FATAL" + ":104:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\":Expected a number or decimal here:")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

    def char_limit(self):
         print(self.file_name + ":" + "FATAL" + ":105:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\":Expected a \' here:")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

    def concat_str(self):
         print(self.file_name + ":" + "FATAL" + ":106:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\":Can only concat a string to a string")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

    def div_by_zero(self):
         print(self.file_name + ":" + "FATAL" + ":107:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\":Divide by zero:")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

    def semi_colon(self):
         print(self.file_name + ":" + "FATAL" + ":108:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\":Expected a ; :")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

    def var_not_reached(self):
         print(self.file_name + ":" + "FATAL" + ":109:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\":The variable is either out of reach or doesnot exist:")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

    def redeclaration(self):
         print(self.file_name + ":" + "FATAL" + ":110:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\":Redeclaration of variable:")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

    def mismatched_values(self):
         print(self.file_name + ":" + "FATAL" + ":111:" + str(self.line_num) +
               ":"+str(self.col_no)+": In line \"" + self.line + "\":The value types and the value doesn't match:")
         print(self.line)
         x = 0
         strx = ""
         while x <= self.col_no - 1:
            strx+=" "
            x+=1
         strx+= "^"
         while x <= len(self.line):
            strx+="~"
            x+=1
         print(strx)

#implementation of stack
class Stack:
    def __init__(self):
        self.items = []

    def push(self, item):
        self.items.append(item)

    def pop(self):
        self.items.pop()

    def peek(self):
        return self.items[-1]

    def is_empty(self):
        return len(self.items) == 0

    def size(self):
        return len(self.items)
    
    def top(self):
        return self.items[self.size() - 1]

class Queue:
    def __init__(self) -> None:
        self.items = []

    def size(self):
        return len(self.items)
    
    def front(self):
        return self.items[0] if self.size() > 0 else None
    
    def push(self, item):
        self.items.append(item)

    def pop(self):
        self.items.pop()

    def is_empty(self):
        return len(self.items) == 0
    
    def pop_front(self):
        self.items.pop(0)

class Tokens:
    def __init__(self, type, value=None) -> None:
        self.type = type
        self.value = value
    
    def __repr__(self) -> str:
        _str = self.type
        if self.value != None:
            _str += " : "+self.value
        return _str


class Lexer:
    def __init__(self, line) -> None:
        self.line = line
        self.current = self.line[0]
        self.tok_list = []
        self.col = 0
        global global_line
        global_line = self.line
    
    def update(self):
        global col_no
        self.col += 1
        self.current = self.line[self.col] if self.col < len(self.line) else None

    def lex(self):
        global line_no
        global col_no
        quote = False
        while self.current != None:
            if self.current in " \n\t":
                self.update()
            elif self.current in alpha or self.current == "_" and self.current != None:
                temp_str = self.current
                self.update()
                while self.current != None and self.current in alpha or self.current == "_":
                    temp_str += self.current
                    self.update()
                if get_key_type(temp_str) != ERROR:
                    self.tok_list.append(Tokens(get_key_type(temp_str), temp_str))
                elif is_identifier_valid(temp_str) != ERROR:
                    self.tok_list.append(Tokens(IDENTIFIER, temp_str))
                else:
                    Error(line_no, col_no, UNKNOWN_TOKEN, self.line)
                    return ERROR
            elif self.current in num_constants and self.current != None:
                temp_str = self.current
                self.update()
                while self.current != None and self.current in num_constants or self.current == ".":
                    temp_str+=self.current
                    self.update()
                if get_num_type(temp_str) != ERROR:
                    self.tok_list.append(Tokens(get_num_type(temp_str), temp_str))
                else:
                    Error(line_no, col_no, INCORRECT_NUMBER, self.line)
                    return ERROR
            elif self.current in opers and self.current != None :
                temp_str = self.current
                self.update()
                if temp_str in "\"\'":
                    if quote == True:
                        quote = False
                    else:
                        quote = True
                if temp_str in "+-/*%!=<>|&" and self.current != None  and self.current in "+-/*%!=<>|&" and quote == False:
                    temp_str+=self.current
                    self.update()
                oper_type = get_oper_type(temp_str)
                if oper_type != ERROR:
                    self.tok_list.append(Tokens(oper_type, temp_str))
                else:
                    Error(line_no, col_no, UNKNOWN_OPER, self.line)
                    return ERROR
                if quote == True:
                    if temp_str == "\"":
                        val = ""
                        while self.current != '\"' and self.current != None :
                            val += self.current
                            col_no += 1
                            self.update()
                        if self.current != '\"':
                            Error(line_no, col_no, EOF_REACHED, self.line)
                            return ERROR
                        else:
                            self.tok_list.append(Tokens(STRING_VAL, val))
                    elif temp_str == '\'':
                        val = ""
                        val += self.current
                        self.update()
                        col_no += len(val)
                        if self.current != '\'':
                           Error(line_no, col_no, CHAR_LIMIT, global_line)
                           return ERROR
                        else:
                            self.tok_list.append(Tokens(CHAR_VAL, val))
            else:
                Error(line_no, col_no, UNKNOWN_TOKEN, self.line)
                return ERROR
            col_no = self.col
        col_no = 0
        return self.tok_list, SUCCESS
    

class Node:
    def __init__(self, type, list_of_toks) -> None:
        self.type = type
        self.toks = list_of_toks

class Variable:
    def __init__(self, name , type, value, scope) -> None:
        self.name = name
        self.type = type
        self.value = value
        self.scope = scope

class SymbolTable:
    def __init__(self) -> None:
        self.var_list = []
        self.function_list = []
    
    def _variable_(self, name, type, value, scope = GLOBAL):
        self.var_list.append(Variable(name, type, value, scope))

    def _is_var_there_(self, name):
        for x in self.var_list:
            if x.name == name:
               return True
        return False
    
    def _get_variable_value(self, name):
        for x in self.var_list:
            if x.name == name:
                return x.value
        return ERROR

global_symbol_table = SymbolTable()

class ExprParser:
    def __init__(self, list_of_toks:list[Tokens]) -> None:
        self.toks = list_of_toks
        self.oper_stack = Stack()
        self.final_stack = Queue()

    def parseExpr(self): #add the functionality to be able to search SymbolTable for the values of variables after SymbolTable is implemented
        global col_no, global_line
        ind = 0
        jump = False
        current = self.toks[ind]
        while ind < len(self.toks):
            if jump == True:
                jump = False
                col_no += len(current.value)
                ind +=1
                if ind < len(self.toks):
                   current = self.toks[ind]
                continue
            elif current.type == NUM or current.type == DECI:
                self.final_stack.push(current)
            elif (self.toks[0].value == '-' and ind == 0) or (current.value == '-' and ind >= 1 and self.oper_stack.size() > 0 and self.toks[ind-1].value == '('):
                self.final_stack.push(Tokens(NUM, "0"))
                if self.toks[ind+1].value in num_constants:
                    self.final_stack.push(self.toks[ind + 1])
                    self.final_stack.push(Tokens("UNARY_MINUS", ";-"))
                    jump = True
                elif self.toks[ind+1].value in "+-/*^%" or self.toks[ind-1].value in "+-/*^%":
                    Error(line_no, col_no, EXPECTED_NUMBER, global_line)
                    return ERROR
                else:
                    self.oper_stack.push(Tokens(MINUS_TOKEN, '-'))
            elif current.value == "--" or current.value == "++" or  current.value == "&&" or current.value == "||" or current.value == "and" or current.value == "or" or current.value in "(+-/*%^":
                if current.value == '(':
                    self.oper_stack.push(current)
                elif current.type == DEC:
                    self.oper_stack.push(Tokens(MINUS_TOKEN, "-"))
                    self.final_stack.push(Tokens(NUM, "1"))
                elif current.value == '++':
                    self.oper_stack.push(Tokens(PLUS_TOKEN, "+"))
                    self.final_stack.push(Tokens(NUM, "1"))
                else:
                    while not self.oper_stack.is_empty() and self.oper_stack.size() > 0 and (current.value in opers or current.value == "&&" or current.value == "||" or current.value == "and" or current.value == "or") and (operator_precedence(self.oper_stack.top().value) >= operator_precedence(current.value)):
                        self.final_stack.push(self.oper_stack.top())
                        self.oper_stack.pop()
                    self.oper_stack.push(current) 
            elif current.value == ')':
                while self.oper_stack.size() > 0 and self.oper_stack.top().value != '(':
                   while not self.oper_stack.is_empty() and self.oper_stack.top().value != '(' and current.value in opers and (operator_precedence(self.oper_stack.top().value) >= operator_precedence(current.value)):
                       self.final_stack.push(self.oper_stack.top())
                       self.oper_stack.pop()
                if self.oper_stack.is_empty() == False:
                    self.oper_stack.pop()
                else:
                    Error(line_no, col_no, STRAY_PAREN, global_line)
                    return ERROR
            elif current.type == IDENTIFIER or current.type == STRING_VAL or current.type == CHAR_VAL or current.type == BOOL_FALSE or current.type == BOOL_TRUE:
                self.final_stack.push(current)
            col_no += len(current.value)
            ind +=1
            if ind < len(self.toks):
                current = self.toks[ind]
        while not self.oper_stack.is_empty():
            self.final_stack.push(self.oper_stack.top())
            self.oper_stack.pop()
        if self.toks[len(self.toks) - 1].type == EOS:
            self.final_stack.push(Tokens(EOS, ";"))
        return self.final_stack, SUCCESS
    
    def solve(self, right, left, oper):
        global global_line, col_no
        right_val = None
        left_val =None
        concat = False
        if right.type == NUM:
            right_val = int(right.value)
        elif right.type == DECI:
            right_val = float(right.value)
        elif right.type == STRING_VAL or right.type == CHAR_VAL:
            concat = True
            right_val = right.value
        elif right.type == BOOL_TRUE or right.type == BOOL_FALSE:
            if right.type == BOOL_TRUE:
                right_val = 1
            else:
                right_val = 0
        elif right.type == IDENTIFIER:
            if global_symbol_table._is_var_there_(right.value):
                right_val = global_symbol_table._get_variable_value(right.value)
            else:
                Error(line_no, col_no, VAR_OUT_OF_SCOPE_OR_NOT_EXIST, global_line)
                return ERROR
        if left.type == NUM:
            left_val = int(left.value)
        elif left.type == DECI:
            left_val = float(left.value)
        elif left.type == STRING_VAL or left.type == CHAR_VAL:
            concat = True
            left_val = left.value
        elif left.type == BOOL_TRUE or left.type == BOOL_FALSE:
            if left.type == BOOL_TRUE:
                left_val = 1
            else:
                left_val = 0
        elif left.type == IDENTIFIER:
               if global_symbol_table._is_var_there_(left.value):
                    left_val = global_symbol_table._get_variable_value(left.value)
               else:
                    Error(line_no, col_no, VAR_OUT_OF_SCOPE_OR_NOT_EXIST, global_line)
                    return ERROR
        if concat == True and right.type == STRING_VAL or left.type == STRING_VAL or left.type == CHAR_VAL or right.type == CHAR_VAL:
            if right.type != left.type != STRING_VAL or right.type != left.type != CHAR_VAL:
                Error(line_no, col_no, CONCAT_ONLY_STR, global_line)
                return ERROR
            return Tokens(STRING_VAL if right.type == STRING_VAL else CHAR_VAL, right_val + left_val)
        elif oper.value in opers or oper.value == ';-':
            if oper.value == "+":
                return Tokens(get_num_type(str(left_val + right_val)), str(left_val + right_val))
            elif oper.value == "-" or oper.value == ';-':
                return Tokens(get_num_type(str(right_val - left_val)), str(right_val - left_val))
            elif oper.value == "*":
                return Tokens(get_num_type(str(right_val * left_val)), str(right_val * left_val))
            elif oper.value == "/":
                if left_val == 0:
                    Error(line_no, col_no, DIV_BY_ZERO, global_line)
                    return ERROR 
                return Tokens(get_num_type(str(right_val / left_val)), str(right_val / left_val))
            elif oper.value == "%":
                return Tokens(get_num_type(str(right_val % left_val)), str(right_val % left_val))
            elif oper.value == "^":
                return Tokens(get_num_type(str(right_val ** left_val)), str((right_val ** left_val)))
        elif oper.value == "&&" or oper.value == "||" or oper.value == "and" or oper.value == "or":
            if oper.value == "&&" or oper.value == "and":
                ans = None
                if right_val == 1 and left_val == 1:
                    ans  = BOOL_TRUE
                else:
                    ans = BOOL_FALSE
                val = None
                if ans == BOOL_TRUE: val = "true" 
                else: val = "false"
                return Tokens(ans, val)
            elif oper.value == "||" or oper.value == "or":
                ans = None
                if right_val == 1 or left_val == 1:
                    ans  = BOOL_TRUE
                else:
                    ans = BOOL_FALSE
                val = None
                if ans == BOOL_TRUE: val = "true" 
                else: val = "false"
                return Tokens(ans, val)
        else:
             Error(line_no, col_no, CONCAT_ONLY_STR, global_line)
             return ERROR

    def eval(self):
        self.parseExpr()
        final_ans = Stack()
        ind = 0
        current = self.final_stack.front()
        length = len(self.final_stack.items)
        while ind < length:
            if self.final_stack.front().type == IDENTIFIER or self.final_stack.front().type == NUM or self.final_stack.front().type == DECI or self.final_stack.front().type ==  BOOL_TRUE or self.final_stack.front().type == BOOL_FALSE or self.final_stack.front().type ==  STRING_VAL or self.final_stack.front().type == CHAR_VAL:
                final_ans.push(self.final_stack.front())
                self.final_stack.pop_front()
            elif self.final_stack.front().value in "+-/*^%" or self.final_stack.front().value == ';-' or self.final_stack.front().value == "and" or self.final_stack.front().value == "&&" or self.final_stack.front().value == "||" or self.final_stack.front().value == "or":
                left  = final_ans.top()
                final_ans.pop()
                right = final_ans.top()
                final_ans.pop()
                ans = self.solve(right, left, self.final_stack.front())
                if ans != ERROR:
                    final_ans.push(ans)
                elif ans == ERROR:
                    return ERROR
                self.final_stack.pop_front()
            ind += 1
            current = self.final_stack.front()
        return final_ans.top()
                                           
class VariableDeclrParser:
    def __init__(self, toks, scope) -> None:
        self.toks = toks
        self.scope = scope
    
    def parse(self):
        global global_line, line_no, col_no
        ind = 0
        current = self.toks[0]
        name = None
        value = None
        type = None
        if ind == 0 and current.type in types:
            type = current.type
            ind+=1
            current = self.toks[ind]
        if ind == 1 and current.type == IDENTIFIER:
            if global_symbol_table._is_var_there_(current.value):
                Error(line_no, col_no, REDECLARATION, global_line)
                return ERROR
            name = current.value
            ind += 1
            current = self.toks[ind]
        if ind == 2 and current.type == EQUAL or current.type == EOS:
            if current.type == EQUAL:
                ind+=1
                current = self.toks[ind]
            elif current.type == EOS:
                ind += 1
                if type == NUM:
                    value = 0
                elif type == DECI:
                    value = 0.0
                elif type == STRING:
                    value = ""
                elif type == CHAR:
                    value = ''
                global_symbol_table._variable_(name, type, value)
                return
        if ind == 3 and current.type == NUM or current.type == DECI or current.type == STRING_VAL or current.type == CHAR_VAL or current.type == BOOL_FALSE or current.type == BOOL_TRUE:
            if type == NUM and current.type == NUM or type == DECI and current.type == DECI or type == STRING and current.type == STRING_VAL or type == CHAR and current.type == CHAR_VAL:
                value = current.value
                ind+=1
            else:
                Error(line_no, col_no, MISMATCHED_VALUES, global_line)
                return ERROR 
        return Tokens(type ,global_symbol_table._get_variable_value(name))

class Parser: #the master parser class 
    def __init__(self, tok_list) -> None:
       self.toks = tok_list
       self.ind = 0
       self.node_list = []

    def parse(self):
        global global_line, col_no, line_no
        if self.toks[0].type == IDENTIFIER or self.toks[0].type == BOOL_FALSE or self.toks[0].type == BOOL_TRUE or self.toks[0].type == NUM or self.toks[0].type == DECI or self.toks[0].type == DOUBLE_QUOTE or self.toks[0].type == SINGLE_QUOTE:
            temp = []
            for x in self.toks:
                temp.append(x)
            self.node_list.append(Node(EXPRESSION_NODE, temp))
        elif self.toks[0].type in types:
            self.node_list.append(Node(VAR_DECLR, self.toks))
        return self.node_list, SUCCESS

class Executor:
    def __init__(self, node_list: list) -> None:
        self.nodes = node_list
        
    def execute(self):
        ind = 0
        current = self.nodes[0]
        while ind < len(self.nodes):
            if current.type == EXPRESSION_NODE:
                ans  = ExprParser(current.toks).eval()
                if ans != ERROR:
                    return ExprParser(current.toks).eval()
                else:
                    return ERROR
            elif current.type == VAR_DECLR:
                ans = VariableDeclrParser(current.toks, GLOBAL).parse()
                if ans != ERROR:
                    return ans
                else:
                    return ERROR


