# SillyQL
Overview
A relational database is the most common means of data storage and retrieval in the modern age. A
relational database model stores data into one or more tables, where each table has columns and
rows. The columns represent a value that can be attributed to an entry (such as “color”, “name”, or “ID”)
and the rows represent individual entries or records (such as “Paoletti”, “my cat”, or “BBB 1695”). You
may find it helpful to think about rows as objects and columns as descriptors for those objects. For
instance, the table pictured to the right has each row corresponding to a car (a data type), and the
columns group a car’s vendors, model, etc. such that this information can be easily retrieved. Rows in
relational databases are also called records or tuples, but in this project we will use the terminology
“row” for consistency.

Program Arguments
silly should accept the following (both optional) command line arguments:
● -h, --help
This causes the program to print a helpful message about how to use the program and then
immediately exit(0).
● -q, --quiet
This causes the program to run in quiet mode. In quiet mode, your program will run very
similarly to normal, except that it will print only numerical summaries of the rows affected by the
command, not any of the actual data. Quiet mode exists so that we may stress test your
program without overloading the autograder with too much output. This flag only affects the
JOIN and PRINT commands and specific instructions for quiet mode output is given for these
commands. Otherwise, if there is no mention of quiet mode with respect to a given piece of
output, you may assume it is always printed. You can implement this feature last; with a
well-built project, adding this functionality should be very simple.

Table Manipulation Commands
In the following commands, output examples are given. These examples are cumulative throughout the
table manipulation command part of the spec (i.e. the table created in the example output for the
CREATE command is the same table that DELETE is performed on during the delete command, and so
on, so you can follow the state of the table throughout the spec).

CREATE - add a new table to the database
Syntax: CREATE <tablename> <N> <coltype1> <coltype2> ... <coltypeN> <colname1> <colname2>
... <colnameN>
Creates a new table with <N> columns (where N > 0). Each column contains data of type <coltype>
and is accessed with the name <colname>. Table names and column names are guaranteed to be
space-free. No two columns in the same table will have the same name (you do not need to check).
Valid data types for coltype are {double, int, bool, string}. This table is initially empty.
Output: Print the following on a single line followed by a newline:
New table <tablename> with column(s) <colname1> <colname2> ... <colnameN> created
Possible errors:
1. A table named <tablename> already exists in the database
Given the following as input:
CREATE 281class 3 string string bool emotion person Y/N
The output should be:
New table 281class with column(s) emotion person Y/N created

INSERT INTO - insert new data into table
Syntax:
INSERT INTO <tablename> <N> ROWS
<value11> <value12> ... <value1M>
<value21> <value22> ... <value2M>
...
<valueN1> <valueN2> ... <valueNM>
Inserts <N> new rows (where N > 0) into the table specified by <tablename>. The number of values in
each line after the first, or M in the example, is guaranteed to be equal to the number of columns in the
table. The first value, <value11>, should be inserted into the first column of the table in the first
inserted row, the second value, <value12>, into the second column of the table in the first inserted row,
and so on. Additionally, the types of the values are guaranteed to be the same as the types of the
columns they are inserted into. For example, if the second column of the table contains integers,
<value2> is guaranteed to be an int. Further, string items are guaranteed to be a single string of
whitespace delimited characters (i.e. "foo bar" is invalid, but "foo_bar" is acceptable).
Output: Print the message shown below, followed by a newline, where <N> is the number of rows
inserted, <startN> is the index of the first row added in the table, and <endN> is the index of the last
row added to the table, 0 based. So, if there were K rows in the table prior to insertion, <startN> = K,
and <endN> = K + N - 1.
Added <N> rows to <tablename> from position <startN> to <endN>
Possible errors:
1. <tablename> is not the name of a table in the database
Given the following as input:
INSERT INTO 281class 8 ROWS
happy Darden true
stressed students false
busy office_hours true
stressed students true
stressed Paoletti true
happy Darden true
happy Sith true
victorious Sith true
The output should be:
Added 8 rows to 281class from position 0 to 7

DELETE FROM - delete specific data from table
Syntax: DELETE FROM <tablename> WHERE <colname> <OP> <value>
Deletes all rows from the table specified by <tablename> where the value of the entry in <colname>
satisfies the operation <OP> with the given value <value>. You can assume that <value> will always
be of the same type as <colname>. For example, to delete all rows from table1 where the entries in
column name equal “John”, the command would be: DELETE FROM table1 WHERE name = John. Or,
to delete all rows from tableSmall where the entries in column size are greater than 15, the
command would be: DELETE FROM tableSmall WHERE size > 15. For simplicity, <OP> is strictly
limited to the set { <, > , = }.
Output (with example): Print the number of rows deleted from the table as shown below, followed by a
newline:
Deleted <N> rows from <tablename>
Possible errors:
1. <tablename> is not the name of a table in the database.
2. <colname> is not the name of a column in the table specified by <tablename>
Given the following as input:
DELETE FROM 281class WHERE person = Darden
The output should be:
Deleted 2 rows from 281class
The search is case sensitive (which makes it easier to code): if we had deleted WHERE person =
darden, no rows would have been removed.

GENERATE INDEX - create a search index on the specified column
Syntax: GENERATE FOR <tablename> <indextype> INDEX ON <colname>
Directs the program to create an index of the type <indextype> on the column <colname> in the table
<tablename>, where <indextype> is strictly limited to the set {hash, bst}, denoting a hash table
index and a binary search tree index respectively. Given the <indextype> hash on column
<colname>, the program should create a hash table that allows a row in the table to be found rapidly
given a particular value in the column <colname>. Given the <indextype> bst on column <colname>,
the program should create an binary search tree that allows rows in the table to be found rapidly given
a particular value in the column <colname>. Only one user-generated Index may exist per table, at
any one time. If an index is requested on a table that already has one, discard the old index before
building the new one.
When bst is the specified index type, you should make use of a std::map<>; when hash is the
specified index type, you should utilize a std::unordered_map<>. It is acceptable for both types to
exist at the same time, but only one (at most) should be in use at any given time (i.e. contain data).
An index is a tool used in databases to speed up future commands. A hash index creates a hash table,
which associates values in a specified column with a collection of rows in the table for which the index
was created. Similarly, a bst index creates a binary search tree which associates values in a specified
column with a collection of rows. Both are useful for different types of commands. In order to get the
correct output in all cases, you must use an index when it is appropriate. Further, you must remember
to update your indices upon edits to the table. bst indices are ordered by operator< for the type in
the specified column.
Output: Print the message shown below, followed by a newline
Created <indextype> index for table <tablename> on column <colname>
Possible errors:
1. <tablename> is not the name of a table in the database.
2. <colname> is not the name of a column in the table specified by <tablename>
Given the following as input:
GENERATE FOR 281class hash INDEX ON emotion
The output should be:
Created hash index for table 281class on column emotion
Note that in this example, the user program should now have created an index of type hash table that
maps from specific emotions of type string to rows in the table 281class,allowing the program to
search for all rows where emotion = happy quickly, among other things.

PRINT - print specified rows
Syntax: PRINT FROM <tablename> <N> <print_colname1> <print_colname2> ... <print_colnameN>
[WHERE <colname> <OP> <value> | ALL ]
Directs the program to print the columns specified by <print_colname1>, <print_colname2>, ...
<print_colnameN> from some/all rows in <tablename>. If there is no condition (i.e. statement is of the
form PRINT ... ALL), the matching columns from all rows of the table are printed. If there is a
condition (i.e. statement is of the form PRINT ... WHERE <colname> <OP> <value>), only rows,
whose <colname> value pass the condition, are printed. The rules for the conditional portion are the
same as for the DELETE FROM statement. It is not guaranteed that the columns in the command are
listed in the same order as they exist in the table, nor is it guaranteed that all columns will be listed.
The table must be searched as follows to ensure compatibility with the autograder:
1. If no index exists or there is a hash index on the conditional column, the results should be
printed in order of insertion into the table.
2. If a bst index exists on the conditional column, the results should be printed in the order in the
BST (least item to greatest item for std::map<> constructed with the default std::less<>
operator), with ties broken by order of insertion into the table.
Output : Print the names of the specified columns, followed by the values of each of the specified
columns in each row, separated by space. Every line should be followed by a newline.
<print_colname1> <print_colname2> ... <print_colnameN>
<value1rowA> <value2rowA> ... <valueNrowA>
...
<value1rowM> <value2rowM> ... <valueNrowM>
Once all the data has been printed, print the following, followed by a newline, where <N> is the number
of rows printed:
Printed <N> matching rows from <tablename>
In quiet mode, do not print the <print_colname1>s or any of the values. Print only the following:
Printed <N> matching rows from <tablename>
Possible errors:
1. <tablename> is not the name of a table in the database.
2. <colname> is not the name of a column in the table specified by <tablename>
3. One (or more ) of the <print_colname>s are not the name of a column in the table specified by
<tablename> (only print the name of the first such column encountered)
Given the following as input:
PRINT FROM 281class 2 person emotion WHERE Y/N = true
The output should be:
person emotion
office_hours busy
students stressed
Paoletti stressed
Sith happy
Sith victorious
Printed 5 matching rows from 281class
Or in quiet mode:
Printed 5 matching rows from 281class

JOIN - join two tables and print result
Syntax: JOIN <tablename1> AND <tablename2> WHERE <colname1> = <colname2> AND PRINT <N>
<print_colname1> <1|2> <print_colname2> <1|2> ... <print_colnameN> <1|2>
Directs the program to print the data in <N> columns, specified by <print_colname1>,
<print_colname2>, ... <print_colnameN>. The <print_colname>s will be the names of
columns in either the first table <tablename1> or the second table <tablename2>, as specified by the
<1/2> argument directly following each <print_colnameN>.
The JOIN command is unique in that it accesses data from multiple tables. The rules for the conditional
portion are the same as for the DELETE FROM statement except that for the JOIN command, <OP> will
be strictly limited to {=} for simplicity. It is not guaranteed that the columns are listed in the same
order as they exist in the table, nor is it guaranteed that all columns will be listed.
The JOIN must be accomplished as follows in order to insure compatibility with the autograder:
1. Iterate through the first table <tablename1> from beginning to end
2. For each row’s respective <colname1> value in <tablename1>, find matching <colname2>
values in <tablename2>, if any exist
3. For each match found, print the column values in the matching rows in the order specified by
the JOIN command
4. The matching rows from the second table must be selected in the order of insertion into that
table.
5. If no rows in the second table match a row in the first table, that row is ignored from the join.
Output : Print the names of the specified columns, followed by the values of each of the specified
columns in each row, separated by space. Every line should be followed by a newline.
<print_colname1> <print_colname2> ... <print_colnameN>
<value1rowA> <value2rowA> ... <valueNrowA>
...
<value1rowM> <value2rowM> ... <valueNrowM>
Once all the data has been printed, print the following, followed by a newline, where N is the number of
rows printed
Printed <N> rows from joining <tablename1> to <tablename2>
In quiet mode, do not print the <print_colname1>s or any of the values. Print only the following:
Printed <N> rows from joining <tablename1> to <tablename2>
Possible errors:
1. <tablename> is not the name of a table in the database.
2. One (or more ) of the <colname>s or <print_colname>s are not the name of a column in the
table specified by <tablename> (only print the name of the first such column encountered)
Given the following as input:
CREATE pets 3 string bool bool Name likes_cats? likes_dogs?
INSERT INTO pets 2 ROWS
Sith true true
Paoletti true false
JOIN pets AND 281class WHERE Name = person AND PRINT 3 Name 1 emotion 2 likes_dogs? 1
The join specific output should be (Note: the CREATE and INSERT INTO commands will generate their
own output, but for simplicity, they are not included in this example):
Name emotion likes_dogs?
Sith happy true
Sith victorious true
Paoletti stressed false
Printed 3 rows from joining pets to 281class
Or in quiet mode:
Printed 3 rows from joining pets to 281class
Note that the JOIN is case sensitive and does not create a new table.

REMOVE - remove existing table from the database
Syntax:
REMOVE <tablename>
Removes the table specified by <tablename> and all associated data from the database, including any
created index.
Output: Print a confirmation of table deletion, followed by a newline, as follows:
Table <tablename> deleted
Possible errors:
1. <tablename> is not the name of a table in the database
Given the following as input:
REMOVE pets
REMOVE 281class
The output should be:
Table pets deleted
Table 281class deleted

QUIT - exit the program
Syntax:
QUIT
Cleans up all internal data (i.e. no memory leaks) and exits the program. Note that the program must exit
with a return 0 from main().
Output: Print a goodbye message, followed by a newline.
Thanks for being silly!
Possible errors: None, except for lacking a QUIT command. Every interactive session or redirected input
file should end with a QUIT command.

# - comment / no operation (useful for adding comments to command files)
Syntax:
# Any text on a line that begins with # is ignored
Discard any lines beginning with #. This command does not produce any output nor can it generate any
errors.


