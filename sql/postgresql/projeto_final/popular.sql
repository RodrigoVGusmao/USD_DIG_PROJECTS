do $$
declare
	paciente_num integer := 64;
	convenio_num integer := 8;
	contratos_num integer := 256;
	especialidade_num integer := 8;
	medico_num integer := 16;
	especialistas_num integer := 64;
	exames_num integer := 16;
	consulta_num integer := 128;
	count integer := 0;
begin
	count := paciente_num;
	LOOP
		EXIT WHEN count = 0;
		INSERT INTO cliente.paciente VALUES (count, 'Paciente ' || count, random()*99999999, 'email' || count || '@mail.net', 
					timestamp '1950-01-01'+random()*(timestamp '2021-01-01'-timestamp '1950-01-01'));
		count := count - 1;
	END LOOP;

	count := convenio_num;
	LOOP
		EXIT WHEN count = 0;
		INSERT INTO cliente.convenio VALUES (DEFAULT, 'Convênio ' || count, random()*10000);
		count := count - 1;
	END LOOP;

	count := contratos_num;
	LOOP
		EXIT WHEN count = 0;
		INSERT INTO cliente.contratos VALUES (DEFAULT, random()*(paciente_num-1)+1, random()*(convenio_num-1)+1);
		count := count - 1;
	END LOOP;

	count := especialidade_num;
	LOOP
		EXIT WHEN count = 0;
		INSERT INTO funcionarios.especialidade VALUES (DEFAULT, 'Especialidade ' || count);
		count := count - 1;
	END LOOP;

	count := medico_num;
	LOOP
		EXIT WHEN count = 0;
		INSERT INTO funcionarios.medico VALUES (count, 'Médico ' || count, floor(random()*99999999),
					'email' || count || '@mail.net', random()*1199999+800000);
		count := count - 1;
	END LOOP;

	count := especialistas_num;
	LOOP
		EXIT WHEN count = 0;
		INSERT INTO funcionarios.especialistas VALUES (DEFAULT, random()*(medico_num-1)+1, random()*(especialidade_num-1)+1);
		count := count - 1;
	END LOOP;

	count := exames_num;
	LOOP
		EXIT WHEN count = 0;
		INSERT INTO clinica.exames VALUES (DEFAULT, 'Exame ' || count, random()*100000, 'descrição genérica');
		count := count - 1;
	END LOOP;

	count := consulta_num;
	LOOP
		EXIT WHEN count = 0;
		INSERT INTO clinica.consulta VALUES (DEFAULT, random()*(paciente_num-1)+1, random()*(exames_num-1)+1, random()*(convenio_num-1)+1, random()*(medico_num-1)+1,
					timestamp '2015-01-01'+random()*(timestamp '2022-01-01'-timestamp '2015-01-01'));
		count := count - 1;
	END LOOP;
end $$