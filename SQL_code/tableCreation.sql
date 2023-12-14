-- Lenteliu kurimas
CREATE TABLE kategorija(
    kategorijos_id SERIAL PRIMARY KEY,
    pavadinimas VARCHAR(255) NOT NULL UNIQUE
);
 
CREATE TABLE produktas(
    produkto_id SERIAL PRIMARY KEY,
    pavadinimas VARCHAR(255) NOT NULL UNIQUE,
    kaina DECIMAL(6,2) CHECK (kaina > 0),
    kiekis INT DEFAULT 0,
    kategorijos_nr INT,
    FOREIGN KEY (kategorijos_nr) REFERENCES kategorija(kategorijos_id)
);
 
CREATE TABLE klientas(
    kliento_id SERIAL PRIMARY KEY,
    vardas_pavarde VARCHAR(255) NOT NULL UNIQUE,
    lojalumo_taskai INT DEFAULT 0 CHECK (lojalumo_taskai >= 0)
);
 
CREATE TABLE pardavimas(
    pardavimo_id SERIAL PRIMARY KEY,
    kliento_nr INT,
    pardavimo_data TIMESTAMP NOT NULL,
    pajamos DECIMAL(6,2) NOT NULL,
    FOREIGN KEY (kliento_nr) REFERENCES klientas(kliento_id)
);
 
CREATE TABLE pardavimo_informacija(
    pardavimo_informacijos_id SERIAL PRIMARY KEY,
    pardavimo_nr INT,
    produkto_nr INT,
    kiekis INT CHECK (kiekis > 0),
    FOREIGN KEY (pardavimo_nr) REFERENCES pardavimas(pardavimo_id),
    FOREIGN KEY (produkto_nr) REFERENCES produktas(produkto_id)
);
 
-- Indeksu kurimas (greitesnis duomenu radimas, bet letesnis duomenu iterpimas)
CREATE UNIQUE INDEX idx_kliento_id ON klientas(kliento_id);
 
CREATE INDEX idx_kategorijos_nr ON produktas(kategorijos_nr);
 
-- Virtualios lenteles
-- Geriausiai parduodamu produktu lentele (view)
CREATE VIEW geriausiai_parduodami_produktai AS
SELECT produkto_nr, SUM(kiekis) AS parduotu_produktu_kiekis
FROM pardavimo_informacija
GROUP BY produkto_nr
ORDER BY parduotu_produktu_kiekis DESC;
 
-- Daznus klientai
CREATE VIEW daznus_klientai AS
SELECT kliento_id, vardas_pavarde, lojalumo_taskai
FROM klientas
WHERE lojalumo_taskai > 50;
 
-- Materializuotas view
CREATE MATERIALIZED VIEW menesiniu_pardavimu_ataskaita AS
SELECT EXTRACT(MONTH FROM pardavimo_data) AS menuo, SUM(pajamos) AS pajamos
FROM pardavimas
GROUP BY menuo;
 
-- REFRESH MATERIALIZED VIEW menesiniu_pardavimu_ataskaita;
 
-- Trigeriai
-- Produkto kiekio keitimas (funkcija)
CREATE FUNCTION atnaujinti_kieki() RETURNS TRIGGER AS $$
BEGIN
    UPDATE Produktas
    SET kiekis = kiekis - NEW.kiekis
    WHERE produkto_id = NEW.produkto_nr;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;
 
-- Triggerio kurimas
CREATE TRIGGER kiekio_atnaujinimo_trigeris
AFTER INSERT ON pardavimo_informacija
FOR EACH ROW
EXECUTE FUNCTION atnaujinti_kieki();
 
-- Lojalumo tasku keitimas (funkcija)
CREATE FUNCTION lojalumo_tasku_keitimas() RETURNS TRIGGER AS $$
BEGIN
    UPDATE klientas
    SET lojalumo_taskai = lojalumo_taskai + NEW.pajamos / 5
    WHERE kliento_id = NEW.kliento_nr;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;
 
-- Trigerio kurimas
CREATE TRIGGER lojalumo_tasku_keitimo_trigeris
AFTER INSERT ON pardavimas
FOR EACH ROW
EXECUTE FUNCTION lojalumo_tasku_keitimas();
 
-- Kiekio tikrinimo funkcija
CREATE FUNCTION patikrinti_kieki() RETURNS TRIGGER AS $$
BEGIN
    IF NEW.kiekis < 0 THEN
        RAISE EXCEPTION 'Kiekis negali buti mazesnis uz 0';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;
 
-- Trigerio kurimas
CREATE TRIGGER kiekio_tikrinimo_trigeris
BEFORE UPDATE OF kiekis ON produktas
FOR EACH ROW
EXECUTE FUNCTION patikrinti_kieki();