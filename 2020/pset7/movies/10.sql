/* write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.

    Your query should output a table with a single column for the name of each person.
*/

SELECT name FROM
(
SELECT DISTINCT p.id, p.name
FROM people as p
JOIN directors AS d ON d.person_id = p.id
JOIN movies AS m on d.movie_id = m.id
JOIN ratings AS r on r.movie_id = m.id
WHERE r.rating >= 9
);

--done
--1841 rows