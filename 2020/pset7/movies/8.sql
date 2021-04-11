/* write a SQL query to list the names of all people who starred in Toy Story.

    Your query should output a table with a single column for the name of each person.
    You may assume that there is only one movie in the database with the title Toy Story.
*/

SELECT p.name
FROM people AS p
JOIN stars AS s ON s.person_id = p.id
JOIN movies AS m ON s.movie_id = m.id
WHERE m.title = 'Toy Story';

--done