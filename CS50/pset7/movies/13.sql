SELECT name FROM people WHERE id IN (sELECT DISTINCT person_id FROM stars WHERE movie_id IN (SELECT DISTINCT movie_id FROM stars WHERE person_id = (SELECT id FROM people WHERE name LIKE 'Kevin Bacon%' AND birth = 1958)) AND person_id NOT IN (SELECT id FROM people WHERE name LIKE 'Kevin Bacon%' AND birth = 1958));