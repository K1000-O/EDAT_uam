# Version 1.0
# Por lo general no se requiere modificar el fichero

export PGDATABASE:=classicmodels
export PGUSER :=alumnodb
export PGPASSWORD :=alumnodb
export PGCLIENTENCODING:=LATIN9
export PGHOST:=localhost

DBNAME =$(PGDATABASE)
PSQL = psql
CREATEDB = createdb
DROPDB = dropdb --if-exists
PG_DUMP = pg_dump
PG_RESTORE = pg_restore

all: dropdb createdb restore shell
createdb:
	@echo Creando BBDD
	@$(CREATEDB)
dropdb:
	@echo Eliminando BBDD
	@$(DROPDB) $(DBNAME)
	rm -f *.log
dump:
	@echo creando dumpfile
	@$(PG_DUMP) > $(DBNAME).sql
restore:
	@echo restore data base
	@cat $(DBNAME).sql | $(PSQL)  
psql: shell
shell:
	@echo create psql shell
	@$(PSQL)  

allqueries: query1 query2 query3 query4 query5 query6
	@cat query?.log > all_queries.log

query1:
	@echo "/*****************************************/
	@echo "/*	PRACTICAS EDAT 2020/2021			*/
	@echo "/*	@estudiante CAMILO JENE CONDE		*/
	@echo "/*	G-1261								*/
	@echo "/*****************************************/
	@echo " "
	@echo "##### CONSULTA 1:"
	@echo query-1: "Cantidad de dinero gastado por clientes que han comprado el 1940 Ford Pickup Truck." | tee query1.log
	@cat query1.sql | $(PSQL) | tee -a query1.log
query2:
	@echo "/*****************************************/
	@echo "/*	PRACTICAS EDAT 2020/2021			*/
	@echo "/*	@estudiante CAMILO JENE CONDE		*/
	@echo "/*	G-1261								*/
	@echo "/*****************************************/
	@echo " "
	@echo "##### CONSULTA 2:"
	@echo query-2: "Tiempo medio que tarda en enviarse cada tipo de producto." | tee query2.log
	@cat query2.sql | $(PSQL) | tee -a query2.log
query3:
	@echo "/*****************************************/
	@echo "/*	PRACTICAS EDAT 2020/2021			*/
	@echo "/*	@estudiante CAMILO JENE CONDE		*/
	@echo "/*	G-1261								*/
	@echo "/*****************************************/
	@echo " "
	@echo "##### CONSULTA 3:"
	@echo query3: "Empleados que reportan a los empleados que reportan al director." | tee query3.log
	@cat query3.sql | $(PSQL) | tee -a query3.log
query4:
	@echo "/*****************************************/
	@echo "/*	PRACTICAS EDAT 2020/2021			*/
	@echo "/*	@estudiante CAMILO JENE CONDE		*/
	@echo "/*	G-1261								*/
	@echo "/*****************************************/
	@echo " "
	@echo "##### CONSULTA 4:"
	@echo query4: "Oficina que ha vendido mayor cantidad de productos." | tee query4.log
	@cat query4.sql | $(PSQL) | tee -a query4.log
query5:
	@echo "/*****************************************/
	@echo "/*	PRACTICAS EDAT 2020/2021			*/
	@echo "/*	@estudiante CAMILO JENE CONDE		*/
	@echo "/*	G-1261								*/
	@echo "/*****************************************/
	@echo " "
	@echo "##### CONSULTA 5:"
	@echo query5: "Países con al menos una oficina que no ha vendido nada en el 2003." | tee query5.log
	@cat query5.sql | $(PSQL) | tee -a query5.log

