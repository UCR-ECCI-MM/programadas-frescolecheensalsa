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
    print("Lexical error on token: ", token)
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

# ------------------  Code for the lexer --------------------------------------
# ------------------  Code for the parser  ------------------------------------

# Import the library for the syntactical analyzer
import ply.yacc as yacc
# Import to manage dates
from datetime import date, datetime

# Main structures
# First structure: so we can know what sites has every topics.
# It corresponds to a dictionary (map) that associates topics (as the keys) to list of titles of sites (as the values).
# Both topics and titles of sites correspond to strings.
sites_per_topic = {}
# Second structure: so we can know how many visits had a site on every available year.
# It corresponds to a dictionary (map) that associates years (as the keys) to maps that associate titles of sites with number of visits (as the values).
# The years correspond to python years (might be ints), the titles of sites correspond to strings,
# The number of visits corresponds to a double calculated by estimating the number of visits per day and the number of days on that year, then multiplying the two.
visits_per_site_per_year = {}
# Third structure: so we can know how many visits had a topic on every available year.
# It corresponds to a dictionary (map) that associates years (as the keys) to maps that associate topics with number of visits (as the values).
# The years correspond to python years (might be ints), the topics correspond to strings.
# The number of visits corresponds to a double calculated by estimating the number of visits per day and the number of days on that year, then multiplying the two.
visits_per_topic_per_year = {}
# Fourth structure: so we can know how many visits had a region on every available year
# It corresponds to a dictionary (map) that associates years (as the keys) to maps that associate regions with number of visits (as the values).
# The years correspond to python years (might be ints), the regions correspond to strings.
# The number of visits corresponds to a double calculated by estimating the number of visits per day and the number of days on that year, then multiplying the two.
visits_per_region_per_year = {}

# Secondary structures
# Fifth structure: so we can know what topics has every site
# It corresponds to a dictionary (map) that associates titles of sites (as the keys) to list of topics (as the values).
# Both titles of sites and topics correspond to strings.
topics_per_site = {}
# Sixth structure: to store the respective url of each site
# It corresponds to a dictionary (map) that associates titles of sites (as the keys) to urls (as the values).
# Both titles of sites and urls correspond to strings.
url_per_site = {}
# Seventh structure: to store the list of records of each site.
# It corresponds to a dictionary (map) that associates titles of sites (as the keys) to list of records (as the values).
# The titles of sites correspond to strings.
# In this case, a record corresponds to a list of 4 elements, the initial date, the final date, the region and the number of visits, on that order.
# The initial and final dates correspond to python dates, the region corresponds to a string and the number of visits to a double.
records_per_site = {}
# Eighth structure: to store all the possible regions.
# It corresponds to a list of regions.
# The regions correspond to strings.
regions_list = []

# Buffers
# Buffer to temporarily store the topics of each site.
# Corresponds to a list of topics.
# The topics correspond to strings.
topics_buffer = []
# Buffer to temporarily store the records of each site
# Corresponds to a list of records.
# In this case, a record corresponds to a list of 4 elements, the initial date, the final date, the region and the number of visits per day, on that order.
records_buffer = []

# Parsing rules
# Starting rule
def p_start(token):
  'start : OPEN_TAG_DATA topics regions sites CLOSE_TAG_DATA'

# Rules for the main headers
# Rule for the topics header
def p_topics(token):
  'topics : OPEN_TAG_TOPICS topics_list CLOSE_TAG_TOPICS'
  # get the topics from the buffer and put it on the head (token[0])
  # gets a copy NOT a reference
  token[0] = [*topics_buffer]
  # clear the buffer so it doesn´t get repeated results
  topics_buffer.clear()

# Rule for the regions header
def p_regions(token):
  'regions : OPEN_TAG_REGIONS regions_list CLOSE_TAG_REGIONS'

# Rule for the sites header
def p_sites(token):
  'sites : OPEN_TAG_SITES sites_list CLOSE_TAG_SITES'

# Rule for the records header
def p_records(token):
  'records : OPEN_TAG_RECORDS records_list CLOSE_TAG_RECORDS'
  # get the records from the buffer and put it on the head (token[0])
  # gets a copy NOT a reference
  token[0] = [*records_buffer]
  # clear the buffer so it doesn´t get repeated results
  records_buffer.clear()

# Rules for the lists
# Rule for the list of topics
def p_topics_list(token):
  '''topics_list : topic topics_list
                 | topic'''

# Rule for the list of regions
def p_regions_list(token):
  '''regions_list : region regions_list
                  | region'''
  # add the region to the regions list
  regions_list.append(token[1])

# Rule for the list of sites
def p_sites_list(token):
  '''sites_list : site sites_list
                | site'''

# Rule for the list of records
def p_records_list(token):
  '''records_list : record records_list
                  | record'''

# Rules for the "base elements"
# Rule for a topic
def p_topic(token):
  'topic : OPEN_TAG_TOPIC TEXT CLOSE_TAG_TOPIC'
  # add the topic to the topics buffer
  topics_buffer.append(token[2])
  # add the topics to the first structure as well (if it was already there
  # does nothing)
  sites_per_topic.setdefault(token[2], [])

# Rule for a region
def p_region(token):
  'region : OPEN_TAG_REGION TEXT CLOSE_TAG_REGION'
  # get the region and put it on the head (token[0])
  token[0] = token[2]

# Rule for a site
def p_site(token):
  'site : OPEN_TAG_SITE url title topics records CLOSE_TAG_SITE'
  # get local references for more clarity
  url = token[2]
  title = token[3]
  topics = token[4]
  records = token[5]

  # --------------------  Code for the first structure ---------------------- #
  generate_first_structure(topics, title)

  # --------------------  Code for the second structure --------------------- #
  generate_second_or_fourth_structure(records, visits_per_site_per_year, title)

  # --------------------  Code for the third structure ---------------------- #
  generate_third_structure(records, topics)

  # --------------------  Code for the fourth structure --------------------- #
  generate_second_or_fourth_structure(records, visits_per_region_per_year, None)

  # --------------------  Code for the fifth structure ---------------------- #
  generate_fifth_structure(topics, title)

  # --------------------  Code for the sixth structure ---------------------- #
  generate_sixth_structure(title, url)

  # --------------------  Code for the seventh structure -------------------- #
  generate_seventh_structure(records, title)

# Modularized procedure to generate the first structure
def generate_first_structure(topics, title):
  # For loop to move through all the topics (on a single site)
  for topic in topics:
    # add the site to all the current topic it has
    sites_per_topic[topic].append(title)

# Modularized procedure to generate the second or fourth structure
def generate_second_or_fourth_structure(records, structure, variable):
  # for loop to move through all the records (on a single site)
  for record in records:
    
    # get local references for date objects
    ini_date = record[0]
    final_date = record[1]
    visits_per_day = record[3]
    
    # if no variable was given use the region of the current record as the variable
    if variable is None: variable = record[2]

    # for loop to go through all the years covered in this record
    for year in range(ini_date.year, final_date.year+1):
      # if the year is not already there
      if structure.get(year) is None:
        # add the year and then a dictionary with the key/value pair variable and 0.0
        structure.setdefault(year, {variable:0.0})
      # if the year is already there
      else:
        # add the dictionary with the key/value pair variable and 0.0 on that year
        structure.get(year).update({variable:0.0})

      # get the number of visits based on the dates
      visits = visit_amount(year, ini_date, final_date, visits_per_day)

      # add the amount of visits on that variable on that year (adds a new key/value pair containing
      # the variable and the visits it already had plus the newly calculated ones)
      structure[year].update({variable:structure[year][variable] + visits})

# Modularized procedure to generate the third structure
def generate_third_structure(records, topics):
  # for loop to move through all the records (on a single site)
  for record in records:
    
    # get local references for date objects
    ini_date = record[0]
    final_date = record[1]
    visits_per_day = record[3]

    # for loop to go through all the years covered in this record
    for year in range(ini_date.year, final_date.year+1):
      # if the year is not already there
      if visits_per_topic_per_year.get(year) is None:
        # add the year and then an empty dictionary (just so it knows the value is a dictionary)
        visits_per_topic_per_year.setdefault(year, {})
      
      # for loop to move through all the topics (on this site) to add them to
      # the year (in case they are not already there, otherwise does nothing)
      for topic in topics:
        # if the topic is not already added to the year
        if visits_per_topic_per_year[year].get(topic) is None:
          # add the topic to the year with a value of 0.0 (0 visits since it just god added)
          visits_per_topic_per_year[year].update({topic:0.0})
      
      # get the number of visits based on the dates
      visits = visit_amount(year, ini_date, final_date, visits_per_day)
      
      # for loop to move through all the topics (on this site) to add the number
      for topic in topics:
          # add the amount of visits on that topic on that year (adds a new key/value pair containing
          # the topic and the visits it already had plus the newly calculated ones)
        visits_per_topic_per_year[year].update({topic:visits_per_topic_per_year[year][topic] + visits})

# Modularized procedure to generate the fifth structure
def generate_fifth_structure(topics, title):
  # add the site (as the key) with an empty list (as the value)
  topics_per_site.setdefault(title, [])
  # For loop to move through all the topics (on a single site)
  for topic in topics:
    # add the topic to the list of topics from the site
    topics_per_site[title].append(topic)

# Modularized procedure to generate the sixth structure
def generate_sixth_structure(title, url):
  # Add the key/value pair of the title and the url to the structure
  url_per_site.update({title:url})

# Modularized procedure to generate the seventh structure
def generate_seventh_structure(records, title):
  # add the title of the site to the structure with and empty list as its associated value
  records_per_site.setdefault(title, [])

  # for loop to move through all the records (on a single site)
  for record in records:
    # get the number of days that happened between the 2 dates
    difference = record[1] - record[0]
    # multiply the visits per date for the number of days + 1 to get the total visits
    record[3] *= (difference.days + 1)

    # add the record to the list of records on that title
    records_per_site[title].append(record)

# Modularized function to calculate the visits amount based on the dates
def visit_amount(year, ini_date, final_date, visits_per_day): 
  # years have 365 days
  days_in_year = 365

  # if the initial date and the final are the same
  if ini_date.year == final_date.year: days_in_year = (final_date - ini_date).days
  # if the year is the first year
  elif year == ini_date.year: days_in_year = (date(ini_date.year, 12, 31) - ini_date).days
  # if the year is the last year
  elif year == final_date.year: days_in_year = (final_date - date(final_date.year, 1, 1)).days

  # find amount of visits per day on that year
  visits = visits_per_day * days_in_year

  # return the number of visits (in millions)
  return visits

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

# ------------------  Code for the parser  ------------------------------------
# ------------------  Code for ply --------------------------------------------

# ------------------  Code for the execution ----------------------------------

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
# If an IO Error exception was thrown reading the data
except IOError:
    print("Error: Could not open the data")

# ------------------  Code for the execution ----------------------------------

# ------------------  Code for the output file generation ---------------------

# ------------------  Code for the output file generation ---------------------