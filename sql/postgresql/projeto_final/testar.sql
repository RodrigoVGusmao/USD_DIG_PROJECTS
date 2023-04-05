DO $$
BEGIN
	RAISE NOTICE 'dados das tabelas:';
END; $$;

SELECT * FROM cliente.paciente;
SELECT * FROM cliente.convenio;
SELECT * FROM cliente.contratos;
SELECT * FROM funcionarios.especialidade;
SELECT * FROM funcionarios.especialistas;
SELECT * FROM funcionarios.medico;
SELECT * FROM clinica.exames;
SELECT * FROM clinica.consulta;

DO $$
BEGIN
	RAISE NOTICE 'dados das tableas de visualização';
END; $$;

SELECT * FROM cliente.exame_usuario;

DO $$
BEGIN
	RAISE NOTICE 'teste de funções';

	RAISE NOTICE 'obter_consulta:';
END; $$;
SELECT cliente.obter_consulta(25);
SELECT cliente.obter_consulta(1000);
SELECT cliente.obter_consulta(NULL);

DO $$
BEGIN
	RAISE NOTICE 'obter_perfil_cliente:';
END; $$;
SELECT cliente.obter_perfil_cliente(25);
SELECT cliente.obter_perfil_cliente(1000);
SELECT cliente.obter_perfil_cliente(NULL);

DO $$
BEGIN
	RAISE NOTICE 'obter_perfil_medico:';
END; $$;
SELECT funcionarios.obter_perfil_medico(8);
SELECT funcionarios.obter_perfil_medico(1000);
SELECT funcionarios.obter_perfil_medico(NULL);

DO $$
BEGIN
	RAISE NOTICE 'obter_medico_por_especialidade:';
END; $$;
SELECT funcionarios.obter_medico_por_especialidade(3);
SELECT funcionarios.obter_medico_por_especialidade(1000);
SELECT funcionarios.obter_medico_por_especialidade(NULL);

DO $$
BEGIN
	RAISE NOTICE 'centavo_para_real';
END; $$;
SELECT funcionarios.centavo_para_real(8);
SELECT funcionarios.centavo_para_real(1000);
SELECT funcionarios.centavo_para_real(543210);
SELECT funcionarios.centavo_para_real(NULL);

DO $$
BEGIN
	RAISE NOTICE 'obter_salario:';
END; $$;
SELECT funcionarios.obter_salario();

DO $$
BEGIN
	RAISE NOTICE 'obter_preco_exames:';
END; $$;
SELECT funcionarios.obter_preco_exames('01-02-2020');
SELECT funcionarios.obter_preco_exames('25-25-2015');
SELECT funcionarios.obter_preco_exames(NULL);

DO $$
BEGIN
	RAISE NOTICE 'receita:';
END; $$;
SELECT funcionarios.receita('01-02-2020');
SELECT funcionarios.receita('25-25-2015');
SELECT funcionarios.receita(NULL);

DO $$
BEGIN
	RAISE NOTICE 'teste de procedimentos';

	RAISE NOTICE 'atualizar_preco:';
END; $$;
SELECT id, preco FROM clinica.exames WHERE id = 5;
CALL funcionarios.atualizar_preco(5, 5432.10);
SELECT id, preco FROM clinica.exames WHERE id = 5;

DO $$
BEGIN
	RAISE NOTICE 'atualizar_salario:';
END; $$;
SELECT CRM, salario FROM funcionarios.medico WHERE CRM = 3;
CALL funcionarios.atualizar_salario(3, 1234.56);
SELECT CRM, salario FROM funcionarios.medico WHERE CRM = 3;

DO $$
BEGIN
	RAISE NOTICE 'inserir_consulta:';
END; $$;
SELECT * FROM clinica.consulta WHERE id = (SELECT max(id) FROM clinica.consulta);
CALL clinica.inserir_consulta(1, 1, null, null, '02-02-2020');
SELECT * FROM clinica.consulta WHERE id = (SELECT max(id) FROM clinica.consulta);
CALL clinica.inserir_consulta(null, null, null, null, null);
SELECT * FROM clinica.consulta WHERE id = (SELECT max(id) FROM clinica.consulta);

SELECT * FROM log.log;
INSERT INTO clinica.consulta VALUES(123456789, 1, 1, null, null, '12-12-2012');

SELECT * FROM log.log;
UPDATE clinica.consulta SET data = now() WHERE CPF = 123456789;

SELECT * FROM log.log;
DELETE FROM clinica.consulta WHERE CPF = 123456789;

SELECT * FROM log.log;
TRUNCATE TABLE clinica.consulta;

SELECT * FROM log.log;
INSERT INTO cliente.paciente VALUES(123456789, 'teste', 987654321, 'teste@teste.tes', '12-24-2024');

SELECT * FROM log.log;
UPDATE cliente.paciente SET nome = 'teste2' WHERE CPF = 123456789;

SELECT * FROM log.log;
DELETE FROM cliente.paciente WHERE CPF = 123456789;

SELECT * FROM log.log;
TRUNCATE TABLE cliente.paciente;