/* write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.

    Your query should output a table with a single column for the name of each person.
    There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
    Kevin Bacon himself should not be included in the resulting list.
*/

--176 rows

/*
-- Kevin Bacon's people.id
SELECT p.id
FROM people AS p
WHERE p.name = 'Kevin Bacon' AND p.birth = 1958;

-- p.id = 102

-- all movies starring Kevin Bacon
SELECT m.id
FROM movies AS m
JOIN stars AS s ON s.movie_id = m.id
JOIN people AS p ON s.person_id = p.id
WHERE p.name = 'Kevin Bacon' AND p.birth = 1958;
*/

-- all co-stars for Kevin Bacon
SELECT y.name FROM
(
    SELECT DISTINCT p.id, p.name
    FROM people AS p
    JOIN stars AS s ON s.person_id = p.id
    JOIN movies AS m ON s.movie_id = m.id
    WHERE m.id IN
        (
        SELECT m.id
        FROM movies AS m
        JOIN stars AS s ON s.movie_id = m.id
        JOIN people AS p ON s.person_id = p.id
        WHERE p.name = 'Kevin Bacon' AND p.birth = 1958

        )
        AND NOT(p.name = 'Kevin Bacon' AND p.birth = 1958)
) AS y;

