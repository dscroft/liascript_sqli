<!--
author:   David Croft

email:    david.croft@warwick.ac.uk

version:  0.0.1

language: en

narrator: US English Female

comment:  

classroom: false
icon: https://raw.githubusercontent.com/dscroft/liascript_materials/refs/heads/main/assets/logo.svg

scormimport: https://raw.githubusercontent.com/dscroft/liascript_materials/refs/heads/main/sql_injection/macros_sql.md
import: macros_sql.md
-->

# SQL Injection

## What is SQL

Structured Query Language, or SQL, is a relational database solution that has been around for decades. 

- Pronounced S-Q-L or sequel depending on who you ask.
- It is a simple language that allows you to interact with databases.
- It can be used to manage and manipulate data in a database.
  - Our concern in this session is its ability to ask questions of the database.

### Intended Learning Outcomes

1. Understand the use of SQL for querying databases.
2. Understand the risks of SQL injection.

**Pre-requisites**

Experience working with rectangular data (data in rows and columns) is required.

- I.e. Excel. 

No experience writing SQL code is expected or required for this module. 

## Relational databases

Relational databases are a way of storing data in tables.

- Each table has rows and columns.
- Each column represents a different attribute of the data.

  - I.e. Name, Age, Address, etc.
- Each row represents a different record.

  - I.e. A different person, a different vehicle, etc.

For example, the following table represents a list of vehicles.

<!-- data-type="none" -->
| vin | make | model | year | reg | color |
| --- | ---- | ----- | ---- | --- | ----- |
| 3GCPCSE05BG193249 | Chevrolet | Silverado | 2011 | HW21 FKL | Black  |
| 1HGCM82633A123456 | Honda     | Accord    | 2003 | JK12 WZX | Blue   |
| 4T1BE32K44U876543 | Toyota    | Camry     | 2004 | XY34 FSD | Silver |
| 2C3KA43R88H125478 | Chrysler  | 300       | 2008 | GH56 BNB | Gray   |
| 1FTFW1ET0EKE39357 | Ford      | F-150     | 2014 | LM98 QWE | White  |

## SELECT

SQL was deliberately designed to be simple to use.

- As such is has a very simple syntax that approximates normal English.

The most common SQL query is the `SELECT` query which is used to retrieve data from a database.
Looking at the example below:

- `SELECT *` asks for all the columns in the table (`*` means all).
- `FROM vehicles` specifiies that we want to retrieve data from the `vehicles` table.
- `LIMIT 5` limits the number of rows returned to 5.

  - So that we don't get overwhelmed with data.

{{1}}
> **Press the button below the SQL query to see it in action.**
>
> - You can edit the SQL query to see the effect that it has on the information returned.

```sql
SELECT *
FROM vehicles
LIMIT 5;
```
@AlaSQL.eval("#vehicleTableA")

<table id="vehicleTableA" border="1"></table><br>

@AlaSQL.buildTable_vehicles

## Authentication

If you are using the search function on a website or logging in it is quite likely that the website's servers will run a query such as the one below to find the information you are looking for.





### Login

<form action="/login" method="post">
    <label for="username">Username:</label><br>
    <input class="lia-quiz__input" type="text" id="username" name="username"><br>
    <label for="password">Password:</label><br>
    <input class="lia-quiz__input" type="password" id="password" name="password"><br><br>
    <input class="lia-btn--outline" type="submit" value="Login">
</form>

{{1}}
> **Enter the following login details.**
>
> - Username: Moose147
> - Password: hunter2


```sql
SELECT COUNT(*)
FROM users
WHERE username = 'Moose147'
  AND password = 'hunter2';
```
@AlaSQL.eval("#correctLoginTable")

<table id="correctLoginTable" border="1"></table><br>

If try with the wrong password then the query will return 0.

```sql
SELECT COUNT(*)
FROM users
WHERE username = 'Moose147'
  AND password = 'wrongpassword';
```
@AlaSQL.eval("#wrongLoginTable")

<table id="wrongLoginTable" border="1"></table><br>

@AlaSQL.buildTable_users





### a

The issue comes in how the website creates that SQL query.

The simplest and most obvious way is to use string concatenation.

- I.e. we simply stick whatever the user enters on the website onto the query.

```python -Example in python
username = get_username()
password = get_password()

query = "SELECT COUNT(*) FROM users WHERE username = '" + 
        username + "' AND password = '" + password + "';"
```

{{1}}
> **Try entering the login details into the form below and see the resulting SQL query.**
>
> - Username: Moose147
> - Password: hunter2
>
> *<strong class="lia-bold" id="loginQuery"></strong>*


@AlaSQL.login





### b

The problem with this approach is that it does not consider just that the user might enter.

- Specifically, what would happen if the user entered a password that contained from SQL syntax?

{{1}}
> **Try entering the login details into the form below and see the resulting SQL query.**
>
> - Username: Moose147'; --
> - Password: Doesn't matter.
>
> It is important that you enter the username exactly as it is written above.
>
> *<strong class="lia-bold" id="loginQuery"></strong>*

**What do you think is happening here?**

- We entered a single `Moose147'` character. 

  - This closed the username string so we are looking for a username of nothing.

- ';'

    - This finished the query.

- Finally we entered `--`.

   - In SQL this is the syntax for a comment.
   - It means that everything following it not part of the query and should be ignored.
   - I.e. the password check.


What we have done is to trick the website into running a different query than it intended.

Instead of a query that checks the username and password, we have a query that just checks the username and then logs in regardless of the password that was entered.

@AlaSQL.login

![Her daughter is named Help I'm trapped in a driver's license factory.](https://imgs.xkcd.com/comics/exploits_of_a_mom.png "XKCD: \"EXPLOITS OF A MOM\"")





## Data exflitration

If you are using the search function on a website it is quite likely that the website's servers will run a query such as the one below to find the information you are looking for.


For example [](https://vehicleenquiry.service.gov.uk/)

We can abuse the SQL functionality to extract information from any table in the database.

- As long as we match the number of columns in the original query.

In this example we show how a query for extracing information from the vehicles table can be injected to return information from the users table.


```sql
SELECT make, color
FROM vehicles
WHERE reg = '' --';
UNION 
SELECT password, email 
FROM users;
```
@AlaSQL.eval("#queryResults")

<table id="queryResults" border="1"></table><br>

@AlaSQL.buildTable_users
@AlaSQL.buildTable_vehicles



## Extremes

The examples shown so far are the most egregious examples of SQL injection.

Any competent programmer should be able to avoid these.

- SOURCE

However, there are more subtle examples.
A vulnerable query anywhere in the code can potentially be exploited to gain access to the complete contents of a database.
In extreme examples individual letters can be extracted databases based on the time that it takes for a query to run. 
For example, SQL queries that will wait 1 second if the first letter of the password on the first row is between A and M. By timing the response from the server the attacker can determine if the passwords starts A-M or N-Z. 
Repeat with A-F and so on until the first letter is revealed.
Repeat for second letter until whole password is revealed.
Repeat for every row for every password.
Repeat for every column for complete table.
Repeat for every table for complete database.

This would obviously be a slow process but it is very possible and more importantly is achievable with cybersecurity/hacking tools that can automatically identify explore the vulnerable query and automatically perform this data exflitration.







## Example

```sql
SELECT *
FROM users
LIMIT 8;
```
@AlaSQL.eval("#dataTable2a")

<table id="dataTable2a" border="1"></table><br>

<div style = "display:none;">

@AlaSQL.buildTable_users

</div>



## Example

Stores information in tables that can be linked together.

Structured Query Language (SQL) is the standard language for querying these types of databases.

```sql
SELECT *
FROM users
LIMIT 8;
```
@AlaSQL.eval("#dataTable2a")

<table id="dataTable2a" border="1"></table><br>

<div style = "display:none;">

@AlaSQL.buildTable_users

</div>


## Logins

If for example you want to log into a website, it is quite possible that website's servers will run a query such as the one below to see if


## Search 

If you are using the search function on a website it is quite likely that the website's servers will run a query such as the one below to find the information you are looking for.

The problem occurs when the website does not properly sanitise the input from the user. 

Information coming from an outside source should always be treated with suspicion.

The reason being that the user is in control of what information they enter and so the user can enter anything they want.


```sql
SELECT *
FROM vehicles;
```
@AlaSQL.eval("#vehicleTableA")

<table id="vehicleTableA" border="1"></table><br>

Equally we can modify our query to only return the information we are interested in.

```sql
SELECT *
FROM vehicles
WHERE year = 2008;
```
@AlaSQL.eval("#vehicleTableB")

<table id="vehicleTableB" border="1"></table><br>

@AlaSQL.buildTable_vehicles