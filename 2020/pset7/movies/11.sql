/* write a SQL query to list the titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated.

    Your query should output a table with a single column for the title of each movie.
    You may assume that there is only one person in the database with the name Chadwick Boseman.
*/

SELECT m.title
FROM movies AS m
JOIN stars AS s on s.movie_id = m.id
JOIN people AS p on s.person_id = p.id
JOIN ratings as R on r.movie_id = m.id
WHERE p.name = 'Chadwick Boseman' --safe assumption per problem set
ORDER BY r.rating DESC
LIMIT 5;

--done