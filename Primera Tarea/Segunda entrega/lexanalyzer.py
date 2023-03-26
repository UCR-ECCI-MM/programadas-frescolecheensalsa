# Copyright 2023 Alejandro Jimenez, Joseph Valverde, Kenneth Villalobos

# ------------------------------------------------------------
 # lexanalizer.py
 #
 # lexical analyzer for the data file with .xml extension
 # ------------------------------------------------------------

# Import the library for the lexical analyzer
import ply.lex as lex

# ------------------  Code for ply -------------------------------------

# Define a rule so we can track line numbers
def t_newline(token):
    r'\n+'
    token.lexer.lineno += len(token.value)
 
# Define a rule to handle the errors
def t_error(token):
    print("Illegal character '%s'" % token.value[0])
    token.lexer.skip(1)

# A string containing ignored characters (spaces and tabs)
t_ignore  = ' \t'

# ------------------  Code for ply -------------------------------------

# Build the lexer
lexer = lex.lex()

# Give the data to the lexer
# lexer.input(data)
 
# While loop to tokenize the data
while True:
    # Get a token from the lexer using the regex above
    tok = lexer.token()

    # If no token was gotten
    if not tok:
        # Break the loop as there is not more input to tokenize
        break
    
    # Print the token
    print(tok)