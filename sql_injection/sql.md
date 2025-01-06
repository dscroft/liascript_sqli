<!--
author:   David Croft

email:    david.croft@warwick.ac.uk

version:  0.0.1

language: en

narrator: US English Female

comment:  

classroom: false
icon: https://dscroft.github.io/liascript_materials/assets/logo.svg

import: macros_sql.md

@LoginExample
SQL query being run:

<strong id="loginQuery"></strong><strong id="loginComment" style="color: lightgrey"></strong><br>

----------------------------------

<form autocomplete="off"> 
    <label for="username">Username:</label><br>
    <input class="lia-quiz__input" type="text" id="username"><br>
    <label for="password">Password:</label><br>
    <input class="lia-quiz__input" type="password" id="password" value=""><br><br>

    <input class="lia-btn--outline" type="button" id="login" value="Login">
    <span style="display:inline-block; width: 20px;"></span>
    <strong id="status"></strong>
</form>

<script>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate a login form with an SQLi vulnerability and validate it against the ALASQL database.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    function update_query()
    {
        let username = document.getElementById("username").value;
        let password = document.getElementById("password").value;
        
        let query = "SELECT COUNT(*) FROM users WHERE username = '" + username + "' AND password = '" + password + "';";

        // shade the comments
        const reg = /(.*)(--.*)/;
        const match = query.match( reg );
        if( match )
        {
          document.getElementById("loginQuery").textContent = match[1];
          document.getElementById("loginComment").textContent = match[2];
        }
        else
        {
          document.getElementById("loginQuery").textContent = query;
          document.getElementById("loginComment").textContent = "";
        }
    }

    function login()
    {
        try 
        {
            let query = document.getElementById("loginQuery").innerHTML;
            let result = alasql(query);

            let status = document.getElementById("status");
            if( result[0]["COUNT(*)"] > 0 )
            {
                status.innerHTML = "Logged in successfully!";
                status.style.color = "green";
            }
            else
            {
                status.innerHTML = "Login failed!";
                status.style.color = "red";
            }
        } 
        catch(e) 
        {
          let error = new LiaError(e.message, 1);
          try {
            let log = e.message.match(/.*line (\d):.*\n.*\n.*\n(.*)/);
            error.add_detail(0, e.name+": "+log[2], "error", log[1] -1 , 0);
          } 
          catch(e){}

          throw error;
        }
      }

    document.getElementById("username").addEventListener("input", update_query);
    document.getElementById("password").addEventListener("input", update_query);
    document.getElementById("login").addEventListener("click", login);

    update_query();
</script>

@end

@SearchExample
SQL query being run:

<strong id="searchQuery"></strong><strong id="searchComment" style="color: lightgrey"></strong><br>

--------------------

<form autocomplete="off"> 
    <input class="lia-quiz__input" type="text" id="registration" placeholder="Enter reg"><br>
    
    <input class="lia-btn--outline" type="button" id="search" value="Search">
    <span style="display:inline-block; width: 20px;"></span>
    <strong id="status"></strong>
</form>

<div id="searchResults"></div><br>

<script>
    function update_query()
    {
        let registration = document.getElementById("registration").value;
        
        let query = "SELECT reg, color, make, model FROM vehicles WHERE reg = '" + registration + "';";

        const reg = /(.*)(--.*)/;
        const match = query.match( reg );
        if( match )
        {
          document.getElementById("searchQuery").textContent = match[1];
          document.getElementById("searchComment").textContent = match[2];
        }
        else
        {
          document.getElementById("searchQuery").textContent = query;
          document.getElementById("searchComment").textContent = "";
        }         
    }

    function search()
    {
        try 
        {
            let query = document.getElementById("searchQuery").innerHTML;
            let result = alasql(query);

            console.log( result )

            let status = document.getElementById("searchResults");
            if( result.length > 0 )
            {
                status.innerHTML = "Results:<br>";
                for( let i=0; i<result.length; i+=1 )
                {
                  console.log( result[i] )
                  status.innerHTML += "<h3>"+result[i].reg+"</h3>"+result[i].color+" "+result[i].make+" "+result[i].model+"<br>";
                }
                status.style.color = "";
            }
            else
            {
                status.innerHTML = "No vehicles found!";
                status.style.color = "red";
            }
        } 
        catch(e) 
        {
          let error = new LiaError(e.message, 1);
          try {
            let log = e.message.match(/.*line (\d):.*\n.*\n.*\n(.*)/);
            error.add_detail(0, e.name+": "+log[2], "error", log[1] -1 , 0);
          } 
          catch(e){}

          throw error;
        }
      }

    document.getElementById("registration").addEventListener("input", update_query);
    document.getElementById("search").addEventListener("click", search);

    update_query();

</script>

@end

@DatabaseDiagram
<!-- class="notranslate" translate="no" style="max-width: 600px"-->
```ascii
+-----------+       +----------+        
| users     |       | owners   |        +----------+       
+-----------+       +----------+        | vehicles |       
| id        +------>| userid   |        +----------+       
| username  |       | vin      +------->| vin      |       
| password  |       +----------+        | make     | 
| firstname |                           | model    |
| lastname  |                           | year     |
| address   |                           | reg      |
| city      |                           | color    |
| county    |                           +----------+
| postal    |     
| phone     |     
| email     |     
+-----------+
```
@end
-->

# SQL Injection

SQL Injection (SQLi) is a type of security vulnerability that occurs when an attacker can manipulate an application's SQL queries by injecting malicious input into fields meant for user-provided data. 
This can lead to unauthorized access to a database, allowing attackers to retrieve, modify, or delete sensitive information.

In the OWASP Top 10, SQL Injection has historically been one of the most critical and common vulnerabilities. 
Although it now falls under the broader "Injection" category rather than it's own, SQL Injection remains one of the most dangerous and frequently exploited injection types.

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

No experience writing SQL code is expected or required for this activity. 


## Relational databases

Relational databases are a way of storing data in tables.

- Each table has rows and columns.
- Each column represents a different attribute of the data.

  - I.e. Name, Age, Address, etc.

- Each row represents a different record.

  - I.e. A different person, a different vehicle, etc.

For example, the vehicles table:

<!-- class="notranslate" translate="no" data-type="none" -->
| vin | make | model | year | reg | color |
| --- | ---- | ----- | ---- | --- | ----- |
| 3GCPCSE05BG193249 | Chevrolet | Silverado | 2011 | HW21 FKL | Black  |
| 1HGCM82633A123456 | Honda     | Accord    | 2003 | JK12 WZX | Blue   |
| 4T1BE32K44U876543 | Toyota    | Camry     | 2004 | XY34 FSD | Silver |
| 2C3KA43R88H125478 | Chrysler  | 300       | 2008 | GH56 BNB | Gray   |
| 1FTFW1ET0EKE39357 | Ford      | F-150     | 2014 | LM98 QWE | White  |

Tables can then be linked together according to their attributes (columns) to describe relationships between records (rows) in different tables.

@DatabaseDiagram





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
> - Press the <i class="icon icon-compile-circle"></i> button below.

```sql
SELECT *
FROM vehicles
LIMIT 5;
```
@AlaSQL.eval("#vehicleTableExample")

<table id="vehicleTableExample" border="1"></table><br>


### Task 1

{{1}}
>  **Try editing the SQL query to see the effect that it has on the information returned.**
>
> - Try changing the limit to see the effect it has on the number of rows returned. 
>
> <script input="submit" default="Press for Hint">
"Try changing 'LIMIT 5' to 'LIMIT 10'."
</script>

<!-- id="queryText" -->
```sql
SELECT *
FROM vehicles
LIMIT 5;
```
@AlaSQL.eval("#vehicleTableTask")

<table id="vehicleTableTask" border="1"></table><br>

{{2}}
> **Try changing the table name.**
>
> <script input="submit" default="Press for Hint">
"Try changing 'vehicles' to 'users'."
</script>

{{3}}
> **Try changing the columns returned.**
>
> <script input="submit" default="Press for Hint">
"Try changing 'SELECT *' to 'SELECT username, password', assuming you are using the users table."
</script>

As a reminder, here is the database structure that we are working with:

@DatabaseDiagram








## Behind the scenes

We have seen that we can use SQL to retrieve data from a database.

- If we can simply connect to the database and run queries then we can retrieve any data that we want.

But the use of SQL is not always so blatant.

- Often it will be used behind the scenes by websites and applications to retrieve information on our behalf.

For example, the search and login pages of a website are likely to use SQL queries to retrieve information from the database.

- The retrieved information is then presented to use in a more user-friendly way.



## Authentication

When we log into a website we commonly supply a username and password.

If our username and password match the details stored in the database then we are allowed to log in.

For example, if our username is "Moose147" and our password is "hunter2" then the following query would be run to check if we can log in.

- `SELECT COUNT(*)` counts the number of rows returned.
- `FROM users` specifies that we are looking in the `users` table.
- `WHERE username = 'Moose147'` specifies that we are looking for a row where the username is "Moose147".
- `AND password = 'hunter2'` specifies that the password column of the row also has to be "hunter2".

------------------------------------------------------------

If the username and password match then the count will be 1, which indicates that this account does exist and they can proceed.

```sql
SELECT COUNT(*)
FROM users
WHERE username = 'Moose147'
  AND password = 'hunter2';
```
@AlaSQL.eval("#correctLoginTable")

<table id="correctLoginTable" border="1"></table><br>

------------------------------------------------------------

If they do not match then the count will be 0, which indicates that this account does not exist, or the password is wrong and they cannot proceed.

```sql
SELECT COUNT(*)
FROM users
WHERE username = 'Moose147'
  AND password = 'wrongpassword';
```
@AlaSQL.eval("#wrongLoginTable")

<table id="wrongLoginTable" border="1"></table><br>






### Task 2

The issue comes in how the (some) programmers creates the SQL query that is being run.

The simplest and most obvious way is to use string concatenation.

- I.e. we simply stick whatever the user enters on the website onto the query.

```python -Example in python (click to open)
username = get_username()
password = get_password()

query = "SELECT COUNT(*) FROM users WHERE username = '" + 
        username + "' AND password = '" + password + "';"
```

```cpp -Example in C++ (click to open)
string username = get_username();
string password = get_password();

string query = "SELECT COUNT(*) FROM users WHERE username = '" + 
               username + "' AND password = '" + password + "';";
```

{{1}}
> **Try entering the login details into the form below.**
>
> This query is being created using string concatenation.
>
> - Username: Moose147
> - Password: hunter2
>
> Press the login button.
>
> <script input="submit" default="Press for Hint">
document.getElementById("username").value = "Moose147";
document.getElementById("password").value = "hunter2";
document.getElementById("username").dispatchEvent(new Event('input'));
"Solution filled in.";
</script>

@LoginExample



### Task 3

The problem with string concatenation is that it does not take into account all the possibilities for what the user might enter.

- Specifically, what would happen if the user entered some SQL?

------------------------------------------------------------

{{1}}
> **Try entering the login details into the form below and see the resulting SQL query.**
>
> - Username: Moose147'; --
> - Password: *Doesn't matter.*
>
> It is important that you enter the username exactly as it is written above.
>
> Press the login button.
>
> <script input="submit" default="Press for Hint">
document.getElementById("username").value = "Moose147'; --";
document.getElementById("username").dispatchEvent(new Event('input'));
"Solution filled in.";
</script>

@LoginExample

{{2}}
> **What do you think is happening here?**
> 
> - We entered a single `Moose147'` character. 
> 
>   - This closed the username string so we are looking for a username of nothing.
> 
> - ';'
> 
>     - This finished the query.
> 
> - Finally we entered `--`.
> 
>    - In SQL this is the syntax for a comment.
>    - It means that everything following it not part of the query and should be ignored.
>    - I.e. the password check.
> 
> What we have done is to trick the website into running a different query than it intended.
> 
> Instead of a query that checks the username and password, we have a query that just checks the username and then logs in regardless of the password that was entered.



### Comic

![Her daughter is named Help I'm trapped in a driver's license factory.](https://imgs.xkcd.com/comics/exploits_of_a_mom.png "XKCD: \"EXPLOITS OF A MOM\"")

The famous XKCD Bobby Tables comic.

`DROP TABLE` is an SQL command that deletes and entire table, in this case the students table.
This activity will let you drop tables in the database if you send the command, but it will also restore the database every time your go to a new page so that you can keep doing the tasks.

For more information see [explainxkcd.com](https://www.explainxkcd.com/wiki/index.php/327:_Exploits_of_a_Mom).



## Data exflitration

If you are using the search function on a website it is quite likely that the website's servers will run a query such as the one below to find the information you are looking for.

For example: [www.check-mot.service.gov.uk](https://www.check-mot.service.gov.uk/)

We can abuse the SQL functionality to extract information from any table in the database.

- As long as we match the number of columns in the original query.

------------------------------------------------------------

In this example we show how a query for extracting information from the vehicles table can be injected to return information from the users table.

```sql
SELECT reg, color, make, model
FROM vehicles
WHERE reg = 'HW21 FKL';
```
@AlaSQL.eval("#searchResults")

<table id="searchResults" border="1"></table><br>


### Task 4

@SearchExample

{{1}}
> **Search for the vehicle with the registration HW21 FKL.**
>
> <script input="submit" default="Press for Hint">
document.getElementById("registration").value = "HW21 FKL";
document.getElementById("registration").dispatchEvent(new Event('input'));
"Solution filled in.";
</script>

### Task 5

@SearchExample

{{1}}
> **Using what we have learned so far, can you extract information on all the vehicles?**
> 
> <script input="submit" default="Press for Hint">
document.getElementById("registration").value = "HW21 FKL' or 1=1; --";
document.getElementById("registration").dispatchEvent(new Event('input'));
"Solution filled in.";
</script>

### Task 6

This is all well and good but we can do more than just extract information from the vehicles table.

- With the UNION operator we can start pulling data from any table.

This is the key lesson regarding SQL injection.

- A single vulnerable query can be exploited to gain access to the complete contents of a database.

------------------------------------------------------------

```sql
SELECT reg, color, make, model
FROM vehicles
WHERE reg = ''
UNION 
SELECT username, password, email, phone 
FROM users;
```
@AlaSQL.eval("#queryResults")

<table id="queryResults" border="1"></table><br>

------------------------------------------------------------

@SearchExample

<script>
  document.getElementById("registration").value = "' UNION SELECT username, password, email, phone FROM users; --";
  document.getElementById("registration").dispatchEvent(new Event('input'));
  send.clear();
</script>

------------------------------------------------------------





## Extremes

The examples shown so far are the most egregious examples of SQL injection.
Any competent programmer should be able to avoid these.

However, there are more subtle examples.
A vulnerable query anywhere in the code can potentially be exploited to gain access to the complete contents of a database.

------------------------------------------------------------

At the extreme end of the spectrum, individual letters can be extracted from databases based on the time that it takes for a query to run. 

For example login pages do not reveal the actual results of the query to the user. They just return a success or failure message.

However, if the query is vulnerable to SQL injection then an attacker can use the time that it for that success or failure message to appear to determine the contents of the database.

- E.g. SQL queries that will wait 1 second depending on the result.

  - If the first letter of the password on the first row is between A and M. 
- By timing the response from the server the attacker can determine if the passwords starts A-M or N-Z. 
  
  - Repeat with A-F and so on until the first letter is revealed.
  - Repeat for second letter until whole password is revealed.
  - Repeat for every row for every password.
  - Repeat for every column for complete table.
  - Repeat for every table for complete database.

This would obviously be a slow process but it is very possible.
More importantly it is achievable with cybersecurity/hacking tools that can automatically identify explore the vulnerability and perform this data exflitration.

- So any SQLi vulnerability anywhere in your system is potentially a complete compromise of your database.


## Solutions

The most egregious thing regarding SQL injection is that it is a solved problem and has been for decades.

- Instead of using string concatenation to build queries, we can use prepared statements.
- Or we can use an ORM (Object Relational Mapping) library that will handle the SQL for us.
- Or we can use SQL placeholders.

The issue with SQL injection is not that it is difficult to solve, it is an ongoing lack of awareness of the problem and software best practices. 


