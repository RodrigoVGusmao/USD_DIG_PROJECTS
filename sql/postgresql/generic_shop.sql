--PostgreSQL 9.6
--'\\' is a delimiter

--1st layer
create table tb_shop(id_shop int not null primary key, shop varchar(50), city varchar(50), address varchar(50));

create table tb_job(id_job int not null primary key, job varchar(50), salary money);

--2nd layer
create table tb_employee(id_employee int not null, period char(5) not null, name varchar(50), id_shop int, id_job int,
                         primary key(id_employee, period), foreign key (id_shop) references tb_shop(id_shop), foreign key (id_job) references tb_job(id_job));

create table tb_product(id_product int not null, id_shop int not null, product varchar(50), price money,
                        primary key(id_product, id_shop), foreign key (id_shop) references tb_shop(id_shop));

create table tb_provider(id_provider int not null, id_shop int not null, provider varchar(50),
                         primary key(id_provider, id_shop), foreign key (id_shop) references tb_shop(id_shop));

create table tb_client(id_client int not null, id_shop int not null, client varchar(50), price money,
                       primary key(id_client, id_shop), foreign key (id_shop) references tb_shop(id_shop));

--3rd layer
create table tb_material(id_material int not null, id_provider int not null, id_shop int not null, id_product int, material varchar(50), price money,
                         primary key(id_material, id_provider, id_shop), foreign key (id_provider, id_shop) references tb_provider(id_provider, id_shop),
                         foreign key (id_shop) references tb_shop(id_shop));

create table tb_client_bag(id_bag int not null, id_product int not null, id_shop int not null, quantity int,
                           primary key(id_bag, id_product, id_shop), foreign key (id_product, id_shop) references tb_product(id_product, id_shop),
                           foreign key (id_shop) references tb_shop(id_shop));

--1st layer
insert into tb_shop values(1, 'SH1', 'CT6', 'BK-16 ST-26 N-13');
insert into tb_shop values(2, 'SH2', 'CT84', 'BK-67 ST-36 N-26');
insert into tb_shop values(3, 'SH3', 'CT16', 'BK-95 ST-94 N-36');
insert into tb_shop values(4, 'SH4', 'CT38', 'BK-13 ST-7 N-11');
insert into tb_shop values(5, 'SH5', 'CT34', 'BK-54 ST-60 N-40');
insert into tb_shop values(6, 'SH6', 'CT82', 'BK-49 ST-25 N-47');
insert into tb_shop values(7, 'SH7', 'CT78', 'BK-76 ST-35 N-36');

insert into tb_job values(1, 'JB1', 520.68);
insert into tb_job values(2, 'JB2', 259.07);
insert into tb_job values(3, 'JB3', 834.78);
insert into tb_job values(4, 'JB4', 475.85);

--2nd layer
insert into tb_employee values(1, 'NIGHT', 'Germaine Isabel Ferreira', 1, 1);
insert into tb_employee values(2, 'DAY', 'Rodney Brígida Lovel', 1, 4);
insert into tb_employee values(3, 'NIGHT', 'Blas Lilia Lestrange', 1, 3);
insert into tb_employee values(4, 'NIGHT', 'Raimundo Lotus Jennings', 1, 2);
insert into tb_employee values(5, 'NIGHT', 'Albino Irene Lobo', 1, 4);
insert into tb_employee values(6, 'NIGHT', 'Jewel Emmaline Jacques', 1, 1);
insert into tb_employee values(7, 'NIGHT', 'Isaac Tod Disney', 1, 3);
insert into tb_employee values(8, 'NIGHT', 'Brittania Elwood Martinson', 1, 4);
insert into tb_employee values(9, 'NIGHT', 'Sydney Laura Jean', 1, 4);
insert into tb_employee values(10, 'NIGHT', 'Cristiana Narcisa Ayers', 1, 3);
insert into tb_employee values(11, 'NIGHT', 'Keir Sheree Shepard', 1, 1);
insert into tb_employee values(12, 'DAY', 'Nicodème Théotime Reed', 1, 1);
insert into tb_employee values(13, 'DAY', 'Brígida Danielle Villaverde', 1, 4);
insert into tb_employee values(14, 'NIGHT', 'Dayton Winoc Harrelson', 1, 4);
insert into tb_employee values(15, 'DAY', 'Christian Terence Bissette ', 1, 4);
insert into tb_employee values(16, 'NIGHT', 'Alexus Talita Jefferson', 1, 2);

insert into tb_employee values(17, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);
insert into tb_employee values(18, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);
insert into tb_employee values(19, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);
insert into tb_employee values(20, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);
insert into tb_employee values(21, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);
insert into tb_employee values(22, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);
insert into tb_employee values(23, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);
insert into tb_employee values(24, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);
insert into tb_employee values(25, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);
insert into tb_employee values(26, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);
insert into tb_employee values(27, 'NIGHT', 'Alexus Talita Jefferson', 2, 1);

insert into tb_employee values(28, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(29, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(30, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(31, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(32, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(33, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(34, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(35, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(36, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(37, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(38, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(39, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);
insert into tb_employee values(40, 'NIGHT', 'Alexus Talita Jefferson', 3, 1);

insert into tb_employee values(41, 'NIGHT', 'Alexus Talita Jefferson', 4, 1);
insert into tb_employee values(42, 'NIGHT', 'Alexus Talita Jefferson', 4, 1);
insert into tb_employee values(43, 'NIGHT', 'Alexus Talita Jefferson', 4, 1);
insert into tb_employee values(44, 'NIGHT', 'Alexus Talita Jefferson', 4, 1);
insert into tb_employee values(45, 'NIGHT', 'Alexus Talita Jefferson', 4, 1);
insert into tb_employee values(46, 'NIGHT', 'Alexus Talita Jefferson', 4, 1);
insert into tb_employee values(47, 'NIGHT', 'Alexus Talita Jefferson', 4, 1);
insert into tb_employee values(48, 'NIGHT', 'Alexus Talita Jefferson', 4, 1);
insert into tb_employee values(49, 'NIGHT', 'Alexus Talita Jefferson', 4, 1);
insert into tb_employee values(50, 'NIGHT', 'Alexus Talita Jefferson', 4, 1);

insert into tb_employee values(51, 'NIGHT', 'Alexus Talita Jefferson', 5, 1);
insert into tb_employee values(52, 'NIGHT', 'Alexus Talita Jefferson', 5, 1);
insert into tb_employee values(53, 'NIGHT', 'Alexus Talita Jefferson', 5, 1);
insert into tb_employee values(54, 'NIGHT', 'Alexus Talita Jefferson', 5, 1);
insert into tb_employee values(55, 'NIGHT', 'Alexus Talita Jefferson', 5, 1);

insert into tb_employee values(56, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(57, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(58, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(59, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(60, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(61, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(62, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(63, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(64, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(65, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(66, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(67, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(68, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(69, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(70, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(71, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(72, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(73, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);
insert into tb_employee values(74, 'NIGHT', 'Alexus Talita Jefferson', 6, 1);

insert into tb_employee values(75, 'NIGHT', 'Alexus Talita Jefferson', 7, 1);
insert into tb_employee values(76, 'NIGHT', 'Alexus Talita Jefferson', 7, 1);
insert into tb_employee values(77, 'NIGHT', 'Alexus Talita Jefferson', 7, 1);
insert into tb_employee values(78, 'NIGHT', 'Alexus Talita Jefferson', 7, 1);
insert into tb_employee values(79, 'NIGHT', 'Alexus Talita Jefferson', 7, 1);
insert into tb_employee values(80, 'NIGHT', 'Alexus Talita Jefferson', 7, 1);
insert into tb_employee values(81, 'NIGHT', 'Alexus Talita Jefferson', 7, 1);
insert into tb_employee values(82, 'NIGHT', 'Alexus Talita Jefferson', 7, 1);

select * from tb_shop;
select * from tb_job;
select * from tb_employee;