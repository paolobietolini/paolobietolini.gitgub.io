# SQL Notes

## Learning Resource

**Khan Academy – Intro to SQL**

---

## Lesson 1: Basics of SQL

### 1. Creating a Table and Inserting Data

**Purpose:**
Learn how to create a table, insert records, and retrieve all data.

```sql
CREATE TABLE groceries (
    id INTEGER PRIMARY KEY,
    name TEXT,
    quantity INTEGER
);

INSERT INTO groceries VALUES (1, "Bananas", 4);
INSERT INTO groceries VALUES (2, "Peanut Butter", 1);
INSERT INTO groceries VALUES (3, "Dark chocolate bars", 2);

SELECT * FROM groceries;
```

**Key Concepts:**

* `CREATE TABLE` defines the structure of the table.
* `PRIMARY KEY` uniquely identifies each row.
* `INSERT INTO` adds new records.
* `SELECT *` retrieves all columns and rows.

---

### 2. Querying Data with Conditions and Sorting

**Adding more columns and filtering results:**

```sql
CREATE TABLE groceries (
    id INTEGER PRIMARY KEY,
    name TEXT,
    quantity INTEGER,
    aisle INTEGER
);

INSERT INTO groceries VALUES (1, "Bananas", 4, 7);
INSERT INTO groceries VALUES (2, "Peanut Butter", 1, 2);
INSERT INTO groceries VALUES (3, "Dark Chocolate Bars", 2, 2);
INSERT INTO groceries VALUES (4, "Ice cream", 1, 12);
INSERT INTO groceries VALUES (5, "Cherries", 6, 2);
INSERT INTO groceries VALUES (6, "Chocolate syrup", 1, 4);
```

**Filtering and ordering results:**

```sql
SELECT *
FROM groceries
WHERE aisle > 5
ORDER BY aisle;
```

**Important Clauses:**

* `WHERE` filters rows based on a condition.
* `ORDER BY` sorts results:

  * `ASC` (ascending) is the default
  * `DESC` (descending) reverses the order

---

### 3. Aggregating Data

**Purpose:**
Use aggregate functions to summarize data.

**Common aggregate functions:**

* `COUNT()` – number of rows
* `SUM()` – total value
* `AVG()` – average value
* `MIN()` – smallest value
* `MAX()` – largest value

**Example:**

```sql
SELECT aisle, SUM(quantity)
FROM groceries
GROUP BY aisle;
```

**Key Concept:**

* `GROUP BY` groups rows that share a common value so aggregates can be applied.

---

## Project: Store Database Design

### Project Description

Design a database for a store that sells one category of items (e.g., clothing).
Requirements:

* One table with **at least 5 columns**
* At least **15 items**
* Queries that:

  * Order items by price
  * Show at least one statistic using an aggregate function

---

### Table Creation

```sql
CREATE TABLE clothes (
    id INTEGER PRIMARY KEY,
    name TEXT,
    color TEXT,
    quantity INTEGER,
    price REAL
);
```

---

### Inserting Data

```sql
INSERT INTO clothes VALUES (6,  "Jeans",   "blue",   20, 39.99);
INSERT INTO clothes VALUES (7,  "Jeans",   "black",  15, 42.50);
INSERT INTO clothes VALUES (8,  "Sweater", "gray",   12, 29.99);
INSERT INTO clothes VALUES (9,  "Sweater", "green",  10, 31.99);
INSERT INTO clothes VALUES (10, "Jacket",  "black",   5, 79.99);
INSERT INTO clothes VALUES (11, "Jacket",  "brown",   7, 85.00);
INSERT INTO clothes VALUES (12, "Hoodie",  "white",  18, 24.99);
INSERT INTO clothes VALUES (13, "Hoodie",  "blue",   16, 26.99);
INSERT INTO clothes VALUES (14, "Shorts",  "black",  22, 19.99);
INSERT INTO clothes VALUES (15, "Shorts",  "red",    20, 18.99);
INSERT INTO clothes VALUES (16, "Skirt",   "pink",   14, 27.50);
INSERT INTO clothes VALUES (17, "Dress",   "yellow",  8, 49.99);
INSERT INTO clothes VALUES (18, "Dress",   "black",   6, 59.99);
INSERT INTO clothes VALUES (19, "Shirt",   "white",  25, 21.99);
INSERT INTO clothes VALUES (20, "Shirt",   "blue",   23, 22.99);
```

---

### Queries

**Order items by price (highest to lowest):**

```sql
SELECT *
FROM clothes
ORDER BY price DESC;
```

**Aggregate statistic (total price per item and color):**

```sql
SELECT name, color, SUM(price) AS total_price
FROM clothes
GROUP BY name, color;
```

