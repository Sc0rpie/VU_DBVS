-- Testiniai duom.
INSERT INTO kategorija (pavadinimas) VALUES ('Elektronika'), ('Knygos'), ('Rubai');
 
INSERT INTO produktas (pavadinimas, kaina, kategorijos_nr, kiekis) VALUES ('Nesiojamas kompiuteris', 1000, 1, 50), ('Kliudziau', 20, 2, 100), ('Maikute', 15, 3, 200);
 
INSERT INTO klientas (vardas_pavarde) VALUES ('Vardenis Pavardenis'), ('Ponas Bonas'), ('Peter Griffin');
 
INSERT INTO pardavimas (kliento_nr, pardavimo_data, pajamos) VALUES (1, CURRENT_TIMESTAMP, 1020), (2,CURRENT_TIMESTAMP, 15);
 
INSERT INTO pardavimo_informacija (pardavimo_nr, produkto_nr, kiekis) VALUES (1, 1, 1), (1, 2, 1), (2, 3, 1);