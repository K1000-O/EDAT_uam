-- Consulta que muestra la oficina que ha vendido el mayot número de objetos.
SELECT offices.officecode, 
       Sum(orderdetails.quantityordered) AS Vendidos 
FROM   offices, 
       employees, 
       customers, 
       orders, 
       orderdetails 
WHERE  offices.officecode = employees.officecode 
       AND employees.employeenumber = customers.salesrepemployeenumber 
       AND customers.customernumber = orders.customernumber 
       AND orders.ordernumber = orderdetails.ordernumber 
GROUP  BY offices.officecode 
ORDER  BY vendidos DESC 
LIMIT  1; 
