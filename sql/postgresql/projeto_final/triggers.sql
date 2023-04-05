SET search_path TO log;

--insert
CREATE FUNCTION log_insert_paciente() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'inserir', 'paciente', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_insert_paciente AFTER INSERT ON cliente.paciente FOR EACH ROW
	EXECUTE FUNCTION log_insert_paciente();

CREATE FUNCTION log_insert_contratos() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'inserir', 'contratos', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_insert_contratos AFTER INSERT ON cliente.contratos FOR EACH ROW
	EXECUTE FUNCTION log_insert_contratos();

CREATE FUNCTION log_insert_convenio() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'inserir', 'convenio', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_insert_convenio AFTER INSERT ON cliente.convenio FOR EACH ROW
	EXECUTE FUNCTION log_insert_convenio();

CREATE FUNCTION log_insert_exame() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'inserir', 'exame', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_insert_exames AFTER INSERT ON clinica.exames FOR EACH ROW
	EXECUTE FUNCTION log_insert_exame();

CREATE FUNCTION log_insert_consulta() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'inserir', 'consulta', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_insert_consulta AFTER INSERT ON clinica.consulta FOR EACH ROW
	EXECUTE FUNCTION log_insert_consulta();

CREATE FUNCTION log_insert_medico() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'inserir', 'medico', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_insert_medico AFTER INSERT ON funcionarios.medico FOR EACH ROW
	EXECUTE FUNCTION log_insert_medico();

CREATE FUNCTION log_insert_especialistas() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'inserir', 'especialistas', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_insert_especialistas AFTER INSERT ON funcionarios.especialistas FOR EACH ROW
	EXECUTE FUNCTION log_insert_especialistas();

CREATE FUNCTION log_insert_especialidade() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'inserir', 'especialidade', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_insert_especialidade AFTER INSERT ON funcionarios.especialidade FOR EACH ROW
	EXECUTE FUNCTION log_insert_especialidade();

--delete
CREATE FUNCTION log_delete_paciente() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'delete', 'paciente', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_delete_paciente AFTER DELETE ON cliente.paciente FOR EACH ROW
	EXECUTE FUNCTION log_delete_paciente();

CREATE FUNCTION log_delete_contratos() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'delete', 'contratos', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_delete_contratos AFTER DELETE ON cliente.contratos FOR EACH ROW
	EXECUTE FUNCTION log_delete_contratos();

CREATE FUNCTION log_delete_convenio() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'delete', 'convenio', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_delete_convenio AFTER DELETE ON cliente.convenio FOR EACH ROW
	EXECUTE FUNCTION log_delete_convenio();

CREATE FUNCTION log_delete_exame() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'delete', 'exame', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_delete_exames AFTER DELETE ON clinica.exames FOR EACH ROW
	EXECUTE FUNCTION log_delete_exame();

CREATE FUNCTION log_delete_consulta() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'delete', 'consulta', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_delete_consulta AFTER DELETE ON clinica.consulta FOR EACH ROW
	EXECUTE FUNCTION log_delete_consulta();

CREATE FUNCTION log_delete_medico() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'delete', 'medico', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_delete_medico AFTER DELETE ON funcionarios.medico FOR EACH ROW
	EXECUTE FUNCTION log_delete_medico();

CREATE FUNCTION log_delete_especialistas() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'delete', 'especialistas', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_delete_especialistas AFTER DELETE ON funcionarios.especialistas FOR EACH ROW
	EXECUTE FUNCTION log_delete_especialistas();

CREATE FUNCTION log_delete_especialidade() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'delete', 'especialidade', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_delete_especialidade AFTER DELETE ON funcionarios.especialidade FOR EACH ROW
	EXECUTE FUNCTION log_delete_especialidade();

--update
CREATE FUNCTION log_update_paciente() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'update', 'paciente', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_update_paciente AFTER UPDATE ON cliente.paciente FOR EACH ROW WHEN(OLD.* IS DISTINCT FROM NEW.*)
	EXECUTE FUNCTION log_update_paciente();

CREATE FUNCTION log_update_contratos() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'update', 'contratos', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_update_contratos AFTER UPDATE ON cliente.contratos FOR EACH ROW WHEN(OLD.* IS DISTINCT FROM NEW.*)
	EXECUTE FUNCTION log_update_contratos();

CREATE FUNCTION log_update_convenio() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'update', 'convenio', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_update_convenio AFTER UPDATE ON cliente.convenio FOR EACH ROW WHEN(OLD.* IS DISTINCT FROM NEW.*)
	EXECUTE FUNCTION log_update_convenio();

CREATE FUNCTION log_update_exames() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'update', 'exames', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_update_exames AFTER UPDATE ON clinica.exames FOR EACH ROW WHEN(OLD.* IS DISTINCT FROM NEW.*)
	EXECUTE FUNCTION log_update_exames();

CREATE FUNCTION log_update_consulta() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'update', 'consulta', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_update_consulta AFTER UPDATE ON clinica.consulta FOR EACH ROW WHEN(OLD.* IS DISTINCT FROM NEW.*)
	EXECUTE FUNCTION log_update_consulta();

CREATE FUNCTION log_update_medico() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'update', 'medico', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_update_medico AFTER UPDATE ON funcionarios.medico FOR EACH ROW WHEN(OLD.* IS DISTINCT FROM NEW.*)
	EXECUTE FUNCTION log_update_medico();

CREATE FUNCTION log_update_especialistas() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'update', 'especialistas', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_update_especialistas AFTER UPDATE ON funcionarios.especialistas FOR EACH ROW WHEN(OLD.* IS DISTINCT FROM NEW.*)
	EXECUTE FUNCTION log_update_especialistas();

CREATE FUNCTION log_update_especialidade() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'update', 'especialidade', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_update_especialidade AFTER UPDATE ON funcionarios.especialidade FOR EACH ROW WHEN(OLD.* IS DISTINCT FROM NEW.*)
	EXECUTE FUNCTION log_update_especialidade();

--truncate
CREATE FUNCTION log_truncate_paciente() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'truncate', 'paciente', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_truncate_paciente AFTER TRUNCATE ON cliente.paciente
	EXECUTE FUNCTION log_truncate_paciente();

CREATE FUNCTION log_truncate_contratos() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'truncate', 'contratos', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_truncate_contratos AFTER TRUNCATE ON cliente.contratos
	EXECUTE FUNCTION log_truncate_contratos();

CREATE FUNCTION log_truncate_convenio() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'truncate', 'convenio', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_truncate_convenio AFTER TRUNCATE ON cliente.convenio
	EXECUTE FUNCTION log_truncate_convenio();

CREATE FUNCTION log_truncate_exames() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'truncate', 'exames', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_truncate_exames AFTER TRUNCATE ON clinica.exames
	EXECUTE FUNCTION log_truncate_exames();

CREATE FUNCTION log_truncate_consulta() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'truncate', 'consulta', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_truncate_consulta AFTER TRUNCATE ON clinica.consulta
	EXECUTE FUNCTION log_truncate_consulta();

CREATE FUNCTION log_truncate_medico() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'truncate', 'medico', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_truncate_medico AFTER TRUNCATE ON funcionarios.medico
	EXECUTE FUNCTION log_truncate_medico();

CREATE FUNCTION log_truncate_especialistas() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'truncate', 'especialistas', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_truncate_especialistas AFTER TRUNCATE ON funcionarios.especialistas
	EXECUTE FUNCTION log_truncate_especialistas();

CREATE FUNCTION log_truncate_especialidade() RETURNS trigger
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	INSERT INTO log.log VALUES(DEFAULT, now(), 'truncate', 'especialidade', OLD, NEW);
	RETURN NEW;
END; $$;
CREATE TRIGGER trg_log_truncate_especialidade AFTER TRUNCATE ON funcionarios.especialidade
	EXECUTE FUNCTION log_truncate_especialidade();