-- Consulta que muestra los empleados que reportan a otros empleados que reportan al director.
-- Para ello primero se realiza una consulta anidada de llos empleados que reportan a directores.
-- Los directores son empleados que no reportan a nadie.
SELECT employees.employeenumber, 
       employees.lastname 
FROM   employees 
WHERE  employees.reportsto IN (SELECT employees.employeenumber 
                               FROM   employees 
                               WHERE  employees.reportsto IN (SELECT 
                                      employees.employeenumber 
                                                              FROM   employees 
                                                              WHERE 
                                      employees.reportsto IS NULL)) 
GROUP  BY employees.employeenumber, 
          employees.lastname; 