/* 1************************************************/

select maker, speed from Product join Laptop using(model) where hd >= 10;

/* 2************************************************/

select model, price from PC where model in(select model from Product where maker='B') union 
select model, price from Laptop where  model in(select model from Product where maker='B') union 
select model, price from Printer where model in(select model from Product where maker='B');

/* 3************************************************/

select distinct maker, type from Product
where maker not in(select maker from Product where type='Laptop') and type='PC';

/* 4************************************************/

select distinct maker from Product 
where model in (select model from PC where speed >= 450);

/**************************************************/

select model, price from Printer 
where price in (select max(price) from Printer);

/* 5************************************************/

select avg(speed) from PC;

/* 6************************************************/

select avg(speed) from Laptop where price > 1000;

/* 7************************************************/

select avg(speed) from PC 
where model in (select model from Product where maker='A');

/* 8************************************************/

select distinct maker from Product join
(
select model, count(*) count from PC group by model having count > 1
) as tmp using(model);

/* 9************************************************/

select hd, count(*) count from PC group by hd having count >= 2;

/* 10***********************************************/

select a.model, b.model, a.speed, b.ram from PC a join
(
	select model, speed, ram from PC
) as b where a.model > b.model and a.ram = b.ram and a.speed = b.speed;

/* 11***********************************************/

select model from Laptop where speed < (select min(speed) from PC);

/* 12***********************************************/

select maker, price from Product join 
	(select model, price from Printer where model in 
		(select model from Printer where color='y') 
	and price in 
		(select min(price) from Printer where color='y')) 
	as temp using(model);

/* 13***********************************************/

select temp2.maker, avg(temp2.screen) from Product join
(
	select maker, temp.model, temp.screen from Product join
	(
		select model, screen from Laptop
	) as temp using(model)
) as temp2 using(model) group by maker;

/* 14***********************************************/

select maker, count(model) cnt from Product where type='PC' group by maker having cnt >= 3;

/* 15***********************************************/

select Product.maker, max(price) from Product join PC using(model) group by maker;

/* 16***********************************************/

select speed, avg(price) from PC where speed > 600 group by speed;

/* 17***********************************************/

select maker from Product where model in
(
	select model from PC where speed >= 750
)
and maker in 
(
	select maker from Product where model in
	(
		select model from Laptop where speed >= 750
	)
);

/* 18***********************************************/

create view max_makers as select maker, max(p.speed) from Product 
join (select model, speed from PC where ram = (select min(ram) from PC)) as p using(model) group by maker;

select distinct Product.maker from Product
where type='Printer' and maker in (select maker from max_makers);

/* 19***********************************************/

select avg(avg_price) from
(
	select price avg_price from Laptop where model in (select model from Product where type='Laptop' and maker='A') union all
	select price from PC where model in (select model from Product where type='PC' and maker='A')
) as temp;

/* 20***********************************************/

select maker, avg(hd) from Product 
join PC using(model) 
where maker in (select distinct maker from Product where type='Printer') 
group by maker;