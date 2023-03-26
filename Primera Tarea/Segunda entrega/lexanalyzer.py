# Copyright 2023 Alejandro Jimenez, Joseph Valverde, Kenneth Villalobos

# ------------------------------------------------------------
 # lexanalizer.py
 #
 # lexical analyzer for the data file with .xml extension
 # ------------------------------------------------------------

# Import the library for the lexical analyzer
import ply.lex as lex

# ------------------  Code for ply -------------------------------------

# A regular expression rules with some action code
# Define a rule to handle the dates
def t_DATE(token):
    # DD-MM-YYYY (01 <= DD <= 31; 01 <= MM <= 12; 1950 <= YYYY <= 2023)
    # D-M-YYYY
    r'((0*[1-9])|([12][0-9])|(3[01]))\-(0*[1-9]|1[0-2])\-(19[5-9][0-9]|20([01][0-9]|2[0-3]))'
    return token

# Define a rule to handle URL´s
def t_URL(token):
    # TODO(Kenneth): Ask and Alejandro and Joseph about this regex and its
    # repercussions, if necessary, change it
    # Https:// (optional) www. (optional) something with letters or numbers . something that does not have space
    r'([a-z]+\:\/\/)?(www\.)?((\w|-)+\.[^ <>]+)'
    return token

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

# Define a rule to rule to read the data
def readData():
    with open('datosSitios.xml', 'r') as file:
        data = file.read()
    return data

# Build the lexer
lexer = lex.lex()

# Read the data
data = readData()

# Give the data to the lexer
lexer.input(data)
 
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