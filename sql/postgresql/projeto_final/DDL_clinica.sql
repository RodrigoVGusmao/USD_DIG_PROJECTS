-- Database criado com:
-- CREATE DATABASE clinica WITH ENCODING 'UTF8' LC_COLLATE='Portuguese_Brazil' LC_CTYPE='Portuguese_Brazil' TEMPLATE template0;
-- usando psql

CREATE SCHEMA clinica;

SET search_path TO clinica;

CREATE TABLE paciente (CPF int primary key not null,
                       nome varchar(50),
                       telefone varchar(21),
                       email varchar(70),
                       data_nasc date);
                       
CREATE TABLE exames (id int primary key not null,
                     nome varchar(20),
                     preco int,
                     descricao varchar(256));
                     
CREATE TABLE convenio (id int primary key not null,
                       nome varchar(20),
                       taxa int);

CREATE TABLE especialidade (id int primary key not null,
                            nome varchar(50));

CREATE TABLE medico (CRM int primary key not null,
                     nome varchar(50),
                     telefone varchar(21),
                     email varchar(70),
                     id_especialidade int references Especialidade(id));

CREATE TABLE consulta (id int primary key not null,
                       CPF int references Paciente(CPF),
                       exame_id int references Exames(id),
                       convenio_id int references Convenio(id),
                       medico_id int references Medico(CRM),
                       data date);

--views
CREATE VIEW exame_usuario AS SELECT A.id, B.nome, C.nome as exame, D.nome as convenio, C.preco, D.taxa, A.data from Consulta A
					right join Paciente B on A.CPF = B.CPF
					right join Exames C on A.exame_id = C.id
					right join Convenio D on A.convenio_id = D.id;