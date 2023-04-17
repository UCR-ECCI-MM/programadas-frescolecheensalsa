# Copyright 2023 Alejandro Jimenez, Joseph Valverde, Kenneth Villalobos

# ------------------------------------------------------------
 # lexsynanalyzer.py
 #
 # lexical and syntactical analyzer for the data file with .xml extension
 # ------------------------------------------------------------

# ------------------  Code for ply -------------------------------------
# Import the library for the lexical analyzer
import ply.lex as lex
from pathlib import Path

# ------------------  Code for the lexer -------------------------------------
# List of token names.   This is always required
tokens = (
    'OPEN_TAG_DATA',               # <data>
    'CLOSE_TAG_DATA',              # </data>

    'OPEN_TAG_TOPICS',             # <topics>
    'CLOSE_TAG_TOPICS',            # </topics>

    'OPEN_TAG_TOPIC',              # <topic>
    'CLOSE_TAG_TOPIC',             # </topic>

    'OPEN_TAG_REGIONS',            # <regions>
    'CLOSE_TAG_REGIONS',           # </regions>

    'OPEN_TAG_REGION',             # <region>
    'CLOSE_TAG_REGION',            # </region>

    'OPEN_TAG_SITES',              # <sites>
    'CLOSE_TAG_SITES',             # </sites>

    'OPEN_TAG_SITE',               # <site>
    'CLOSE_TAG_SITE',              # </site>

    'OPEN_TAG_URL',                # <url>
    'CLOSE_TAG_URL',               # </url>

    'OPEN_TAG_TITLE',              # <title>
    'CLOSE_TAG_TITLE',             # </title>

    'OPEN_TAG_RECORDS',            # <records>
    'CLOSE_TAG_RECORDS',           # </records>

    'OPEN_TAG_RECORD',             # <record>
    'CLOSE_TAG_RECORD',            # </record>

    'OPEN_TAG_INITIAL_DATE',       # <initial-date>
    'CLOSE_TAG_INITIAL_DATE',      # </initial-date>

    'OPEN_TAG_FINAL_DATE',         # <final-date>
    'CLOSE_TAG_FINAL_DATE',        # </final-date>

    'OPEN_TAG_NUMBER_OF_VISITS',   # <number-of-visits>
    'CLOSE_TAG_NUMBER_OF_VISITS',  # </number-of-visits>
    
    'DATE',
    'NUMBER',
    'LINK',
    'TEXT',
)

# Regular expression rules with some action code
# Define a rule to handle the dates
def t_DATE(token):
    # DD-MM-YYYY (01 <= DD <= 31; 01 <= MM <= 12; 1950 <= YYYY <= 2023)
    # D-M-YYYY
    r'((0*[1-9])|([12][0-9])|(3[01]))\-(0*[1-9]|1[0-2])\-(19[5-9][0-9]|20([01][0-9]|2[0-3]))'
    return token

# define a rule to handle the numbers
def t_NUMBER(t):
    r'(\d+(\.\d+)?)(?=(\<\/number\-of\-visits\>))'
    t.value = float(t.value)    
    return t

# Define a rule to handle URL´s or LINKS
def t_LINK(token):
    # Https:// (optional) www. (optional) something with letters or numbers . something that does not have space or brackets
    r'([a-z]+\:\/\/)?(www\.)?((\w|-)+\.[^ <>]+)(?=(\<\/url\>))'
    return token

# Define a rule so we can track line numbers
def t_newline(token):
    r'\n+'
    token.lexer.lineno += len(token.value)
 
# Define a rule to handle the errors
def t_error(token):
    print("Illegal character '%s'" % token.value[0])
    token.lexer.skip(1)

# Regular expression rules for simple tokens
t_OPEN_TAG_DATA=r'\<data\>'
t_CLOSE_TAG_DATA=r'\<\/data\>'

t_OPEN_TAG_TOPICS=r'\<topics\>'
t_CLOSE_TAG_TOPICS=r'\<\/topics\>'

t_OPEN_TAG_TOPIC=r'\<topic\>'
t_CLOSE_TAG_TOPIC=r'\<\/topic\>'

t_OPEN_TAG_REGIONS=r'\<regions\>'
t_CLOSE_TAG_REGIONS=r'\<\/regions\>'

t_OPEN_TAG_REGION=r'\<region\>'
t_CLOSE_TAG_REGION=r'\<\/region\>'

t_OPEN_TAG_SITES=r'\<sites\>'
t_CLOSE_TAG_SITES=r'\<\/sites\>'

t_OPEN_TAG_SITE=r'\<site\>'
t_CLOSE_TAG_SITE=r'\<\/site\>'

t_OPEN_TAG_URL=r'\<url\>'
t_CLOSE_TAG_URL=r'\<\/url\>'

t_OPEN_TAG_TITLE=r'\<title\>'
t_CLOSE_TAG_TITLE=r'\<\/title\>'

t_OPEN_TAG_RECORDS=r'\<records\>'
t_CLOSE_TAG_RECORDS=r'\<\/records\>'

t_OPEN_TAG_RECORD=r'\<record\>'
t_CLOSE_TAG_RECORD=r'\<\/record\>'

t_OPEN_TAG_INITIAL_DATE=r'\<initial\-date\>'
t_CLOSE_TAG_INITIAL_DATE=r'\<\/initial\-date\>'

t_OPEN_TAG_FINAL_DATE=r'\<final\-date\>'
t_CLOSE_TAG_FINAL_DATE=r'\<\/final\-date\>'

t_OPEN_TAG_NUMBER_OF_VISITS=r'\<number\-of\-visits\>'
t_CLOSE_TAG_NUMBER_OF_VISITS=r'\<\/number\-of\-visits\>'

# Strings that might have spaces
t_TEXT=r'[^\<\>]+'

# A string containing ignored characters (spaces and tabs)
t_ignore  = ' \t'



# Build the lexer
lexer = lex.lex()

# ------------------  Code for the lexer -------------------------------------
# ------------------  Code for the parser  -----------------------------------
# Import the library for the syntactical analyzer
import ply.yacc as yacc

# dictionary of structures (Currently empty as it is still not needed)
structures = { }

# Parsing rules
# Starting rule
def p_START(token):
    'START : OPEN_TAG_DATA TOPICS REGIONS SITES CLOSE_TAG_DATA'

# Rules for the main headers
# Rule for the topics header
def p_TOPICS(token):
    'TOPICS : OPEN_TAG_TOPICS TOPICS_LIST CLOSE_TAG_TOPICS'
    # Do something

# Rule for the regions header
def p_REGIONS(token):
    'REGIONS : OPEN_TAG_REGIONS REGIONS_LIST CLOSE_TAG_REGIONS'
    # Do something

# Rule for the sites header
def p_SITES(token):
    'SITES : OPEN_TAG_SITES SITES_LIST CLOSE_TAG_SITES'
    # Do something

# Rule for the records header
def p_RECORDS(token):
    'RECORDS : OPEN_TAG_RECORDS RECORDS_LIST CLOSE_TAG_RECORDS'
    # Do something

# Rules for the lists
# Rule for the list of topics
def p_TOPICS_LIST(token):
    '''TOPICS_LIST : TOPIC TOPICS_LIST
                   |'''
    # Do something

# Rule for the list of regions
def p_REGIONS_LIST(token):
    '''REGIONS_LIST : REGION REGIONS_LIST
                    |'''
    # Do something

# Rule for the list of sites
def p_SITES_LIST(token):
    '''SITES_LIST : SITE SITES_LIST
                  |'''
    # Do something

# Rule for the list of records
def p_RECORDS_LIST(token):
    '''RECORDS_LIST : RECORD RECORDS_LIST
                    |'''
    # Do something

# Rules for the "base elements"
# Rule for a topic
def p_TOPIC(token):
    'TOPIC : OPEN_TAG_TOPIC TEXT CLOSE_TAG_TOPIC'
    # Do something

# Rule for a region
def p_REGION(token):
    'REGION : OPEN_TAG_REGION TEXT CLOSE_TAG_REGION'
    # Do something

# Rule for a site
def p_SITE(token):
    'SITE : OPEN_TAG_SITE URL TITLE TOPICS RECORDS CLOSE_TAG_SITE'
    # Do something

# Rule for a record
def p_RECORD(token):
    'RECORD : OPEN_TAG_RECORD INITIAL_DATE FINAL_DATE REGION VISITS CLOSE_TAG_RECORD'
    # Do something

# Rule for an url
def p_URL(token):
    'URL : OPEN_TAG_URL LINK CLOSE_TAG_URL'
    # Do something

# Rule for a title
def p_TITLE(token):
    'TITLE : OPEN_TAG_TITLE TEXT CLOSE_TAG_TITLE'
    # Do something

# Rule for an initial date
def p_INITIAL_DATE(token):
    'INITIAL_DATE : OPEN_TAG_INITIAL_DATE DATE CLOSE_TAG_INITIAL_DATE'
    # Do something

# Rule for a final date
def p_FINAL_DATE(token):
    'FINAL_DATE : OPEN_TAG_FINAL_DATE DATE CLOSE_TAG_FINAL_DATE'
    # Do something

# Rule for the visits
def p_VISITS(token):
    'VISITS : OPEN_TAG_NUMBER_OF_VISITS NUMBER CLOSE_TAG_NUMBER_OF_VISITS'
    # Do something

# General rules
# Rule for errors
def p_error(token):
    print("Syntax error '%s'" % token)

# Build the parser
parser = yacc.yacc()

# ------------------  Code for the parser  -----------------------------------
# ------------------  Code for ply -------------------------------------------

# Define a rule to rule to read the data
def readData():
    # Made using example from https://stackoverflow.com/questions/40416072/reading-a-file-using-a-relative-path-in-a-python-project
    fileName = Path(__file__).parent / "data.xml"
    with open(fileName, 'r') as file:
        data = file.read()
    return data

try:
    # Try to read the data
    data = readData()
    # If it could read the data, parse it
    parser.parse(data)

    print("\nPara revisar resultados: vea el documento creado parser.out\n")
# If an IO Error exception was thrown reading the data
except IOError:
    print("Error: Could not open the data")
