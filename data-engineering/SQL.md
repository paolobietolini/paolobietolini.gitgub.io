To learn
Window functions

CTEs

Query optimization basics

Writing transformations, not just selects


## IN Operator

### Definition

The `IN` operator is used to check if the value in a column belongs to a specified set of values.

### Purpose

- It avoids the need for multiple `OR` conditions.
- It improves query readability.

### Example

```sql
SELECT * 
FROM exercise_logs 
WHERE type IN ('biking', 'hiking', 'tree climbing', 'rowing');
```

This selects all exercises where the `type` matches one of the values in the list.

It is equivalent to:

```sql
WHERE type = 'biking'
   OR type = 'hiking'
   OR type = 'tree climbing'
   OR type = 'rowing';
```

---

## Subqueries

### Definition

A subquery is a query nested inside another query, enclosed in parentheses.

```sql
SELECT ...
WHERE column IN (SELECT ...);
```

The subquery executes first, and its result is used by the outer query.

---

## IN with Subqueries

### Basic Example

```sql
SELECT * 
FROM exercise_logs 
WHERE type IN (
    SELECT type FROM drs_favorites
);
```

This process involves:
1. The subquery retrieving all favorite `type` values from the doctor.
2. The outer query selecting only exercises matching those types.

It effectively means: "Show exercises that are among the doctor's favorites."

---

## Subquery with Condition

```sql
SELECT * 
FROM exercise_logs 
WHERE type IN (
    SELECT type 
    FROM drs_favorites 
    WHERE reason = 'Increases cardiovascular health'
);
```

This selects only exercise types that improve cardiovascular health in the subquery, then shows matching exercises in the outer query.

Result: Exercises beneficial for cardiovascular health.

---

## Key Rules for Subqueries with IN

### Must Return a Single Column

Valid:
```sql
SELECT type FROM drs_favorites   -- Valid
```

Invalid:
```sql
SELECT type, reason FROM drs_favorites   -- Error
```

### Data Types Must Match

For example:
```sql
exercise_logs.type IN (drs_favorites.type)
```

---

## LIKE in Subqueries

```sql
SELECT * 
FROM exercise_logs 
WHERE type IN (
    SELECT type 
    FROM drs_favorites 
    WHERE reason LIKE '%cardiovascular health%'
);
```

This uses `LIKE` for partial text matches, with `%` as a wildcard for any text before or after.

It selects exercises where the reason contains "cardiovascular health."

Note: Without `%`, `LIKE` behaves like `=`.

---

## SUM Operator

(The notes on this operator are incomplete in the original. SUM is an aggregate function that calculates the total of a numeric column.)

---

## AS Operator

(The notes on this operator are incomplete in the original. AS is used for aliasing columns or tables in queries for better readability.)

---

## HAVING Operator

Compared to `WHERE`, `HAVING` applies conditions to aggregated results after grouping (e.g., with GROUP BY), while `WHERE` filters rows from the original table before aggregation.
---
## AVG Operator
Return the average of a number
--- 
## COUNT Operator
The COUNT() function returns the number of rows that matches a specified criterion.

Find the total number of rows in the Products table:

```SQL
SELECT COUNT(*)
FROM Products;
```


---

## CASE