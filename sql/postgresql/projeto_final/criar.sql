-- Database criado com:
-- CREATE DATABASE clinica WITH ENCODING 'UTF8' LC_COLLATE='Portuguese_Brazil' LC_CTYPE='Portuguese_Brazil' TEMPLATE template0;
-- usando psql

CREATE SCHEMA cliente;
CREATE SCHEMA clinica;
CREATE SCHEMA funcionarios;
CREATE SCHEMA log;

SET search_path TO cliente;

CREATE TABLE paciente (CPF int primary key not null,
                       nome varchar(50),
                       telefone varchar(21),
                       email varchar(70),
                       data_nasc date);
                     
CREATE TABLE convenio (id SERIAL primary key not null,
                       nome varchar(20),
                       taxa int);

CREATE TABLE contratos (id SERIAL primary key not null,
			CPF int not null REFERENCES paciente(CPF),
			id_convenio int not null REFERENCES convenio(id));

SET search_path TO funcionarios;

CREATE TABLE especialidade (id SERIAL primary key not null,
			nome varchar(50));

CREATE TABLE medico (CRM int primary key not null,
			nome varchar(50),
			telefone varchar(21),
			email varchar(70),
			salario integer);

CREATE TABLE especialistas (id SERIAL primary key not null,
			CRM int not null REFERENCES medico(CRM),
			id_especialidades int not null REFERENCES especialidade(id));

SET search_path TO clinica;

CREATE TABLE exames (id SERIAL primary key not null,
			nome varchar(20),
			preco int,
			descricao varchar(256));

CREATE TABLE consulta (id SERIAL primary key not null,
			CPF int not null REFERENCES cliente.paciente(CPF),
			exame_id int not null REFERENCES exames(id),
			convenio_id int REFERENCES cliente.convenio(id),
			medico_id int REFERENCES funcionarios.medico(CRM),
			data date not null);

SET search_path TO log;

CREATE TABLE log (id SERIAL PRIMARY KEY NOT NULL,
            data date not null,
            acao varchar(10),
            tabela varchar(20),
            antigo varchar(300),
            novo varchar(300));

--views
SET search_path TO cliente;
CREATE VIEW exame_usuario AS SELECT A.id, A.CPF, B.nome, C.nome as exame, D.nome as convenio, C.preco, A.data FROM clinica.consulta A
					RIGHT JOIN paciente B on A.CPF = B.CPF
					RIGHT JOIN clinica.exames C on A.exame_id = C.id
					RIGHT JOIN convenio D on A.convenio_id = D.id;

--functions
SET search_path TO cliente;

CREATE FUNCTION obter_consulta(CPF_usuario integer) RETURNS TABLE(id integer, CPF integer, nome varchar(50), exame varchar(20), convenio varchar(20), preco integer, data date)
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$ 
BEGIN
	RETURN QUERY(SELECT * FROM cliente.exame_usuario A WHERE A.CPF = CPF_usuario); 
END; $$;

CREATE FUNCTION obter_perfil_cliente(CPF_usuario integer) RETURNS TABLE (convenio varchar(20), taxa integer, CPF integer, nome varchar(50), telefone varchar(21), email varchar(70), data_nasc date)
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	RETURN QUERY(SELECT C.nome, C.taxa, B.CPF, B.nome, B.telefone, B.email, B.data_nasc FROM cliente.contratos A
		RIGHT JOIN cliente.paciente B on B.CPF = A.CPF
		RIGHT JOIN cliente.convenio C on C.id = A.id_convenio
		WHERE A.CPF = CPF_usuario
		GROUP BY C.nome, C.taxa, B.CPF);
END; $$;

SET search_path TO funcionarios;

CREATE FUNCTION obter_perfil_medico(CRM_medico integer) RETURNS TABLE (especialidade varchar(50), CRM integer, nome varchar(50), telefone varchar(21), email varchar(70), salario integer)
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$
BEGIN
	RETURN QUERY(SELECT C.nome, B.CRM, B.nome, B.telefone, B.email, B.salario FROM funcionarios.especialistas A
		RIGHT JOIN funcionarios.medico B on B.CRM = A.CRM
		RIGHT JOIN funcionarios.especialidade C on C.id = A.id_especialidades
		WHERE A.CRM = CRM_medico
		GROUP BY C.nome, B.CRM);
END; $$;

CREATE FUNCTION obter_medico_por_especialidade(especialidade integer) RETURNS TABLE (CRM integer, nome varchar(50))
LANGUAGE plpgsql
AS $$
BEGIN
	RETURN QUERY(SELECT A.CRM, B.nome FROM funcionarios.especialistas A
		RIGHT JOIN funcionarios.medico B on A.CRM = B.CRM
		WHERE A.id_especialidades = especialidade);
END; $$;

CREATE FUNCTION centavo_para_real (centavos integer) RETURNS decimal
LANGUAGE plpgsql RETURNS NULL ON NULL INPUT
AS $$ 
BEGIN
	RETURN CAST(centavos AS decimal)/100;
END; $$;

CREATE FUNCTION obter_salario() RETURNS TABLE (salario decimal, total decimal)
LANGUAGE plpgsql
AS $$
BEGIN
	RETURN QUERY(SELECT CAST(A.salario AS decimal)/100, CAST((SELECT SUM(A.salario) FROM funcionarios.medico A) AS decimal)/100 FROM funcionarios.medico A
		GROUP BY A.salario);
END; $$;

CREATE FUNCTION obter_preco_exames(data_limite date) RETURNS TABLE (data date, preco decimal, total decimal)
LANGUAGE plpgsql
AS $$
BEGIN
	RETURN QUERY(SELECT A.data, CAST(B.preco AS decimal)/100, CAST((SELECT SUM(B.preco) FROM clinica.consulta A RIGHT JOIN clinica.exames B ON B.id = A.exame_id WHERE A.data >= data_limite) AS decimal)/100 FROM clinica.consulta A
		RIGHT JOIN clinica.exames B ON B.id = A.exame_id
		WHERE A.data >= data_limite
		GROUP BY B.preco, A.data);
END; $$;

CREATE FUNCTION receita(data_limite date) RETURNS TABLE (total decimal)
LANGUAGE plpgsql
AS $$
BEGIN
	RETURN QUERY(SELECT((SELECT DISTINCT A.total FROM obter_preco_exames(data_limite) A) - (SELECT DISTINCT B.total FROM obter_salario() B)));
END; $$;

--procedure
SET search_path TO funcionarios;

CREATE PROCEDURE atualizar_salario(CRM_medico integer, salario_medico real)
LANGUAGE plpgsql
AS $$
BEGIN
	UPDATE funcionarios.medico A SET salario = CAST(salario_medico*100 AS integer) WHERE A.CRM = CRM_medico;
END; $$;

CREATE PROCEDURE atualizar_preco(id_exame integer, preco_exame real)
LANGUAGE plpgsql
AS $$
BEGIN
	UPDATE clinica.exames A SET preco = CAST(preco_exame*100 AS integer) WHERE id_exame = A.id;
END; $$;

SET search_path TO clinica;

CREATE PROCEDURE inserir_consulta(CPF_cliente integer, id_exame integer, id_convenio integer, id_medico integer, data_cliente date)
LANGUAGE plpgsql
AS $$
BEGIN
	INSERT INTO clinica.consulta VALUES(DEFAULT, CPF_cliente, id_exame, id_convenio, id_medico, data_cliente);
END; $$;

--roles
CREATE ROLE medicos;
GRANT SELECT ON ALL TABLES IN SCHEMA clinica TO medicos;

CREATE ROLE recepcionistas;
GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA cliente TO recepcionistas;
GRANT EXECUTE ON PROCEDURE clinica.inserir_consulta TO recepcionistas;
GRANT EXECUTE ON FUNCTION funcionarios.obter_medico_por_especialidade TO recepcionistas;

CREATE ROLE financeiro;
GRANT EXECUTE ON FUNCTION funcionarios.centavo_para_real, funcionarios.obter_salario, funcionarios.obter_preco_exames, funcionarios.receita TO financeiro;
GRANT EXECUTE ON PROCEDURE funcionarios.atualizar_salario, funcionarios.atualizar_preco TO financeiro;

CREATE ROLE administradores;
GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA funcionarios, clinica TO administradores;
GRANT EXECUTE ON ALL FUNCTIONS IN SCHEMA funcionarios, clinica TO administradores;
GRANT SELECT ON ALL TABLES IN SCHEMA log TO administradores;

GRANT EXECUTE ON ALL FUNCTIONS IN SCHEMA cliente TO PUBLIC;