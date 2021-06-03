-- Paises con almenos 1 oficina que no ha vendido nada en 2003.
SELECT offices.country, 
       Count(*) AS nosell 
FROM   offices 
WHERE  offices.country NOT IN (SELECT offices.country 
                               FROM   offices, 
                                      employees, 
                                      customers, 
                                      orders 
                               WHERE  offices.officecode = employees.officecode 
                                      AND employees.employeenumber = 
                                          customers.salesrepemployeenumber 
                                      AND 
              customers.customernumber = orders.customernumber 
                                      AND orders.ordernumber IN 
                                          (SELECT orders.ordernumber 
                                           FROM   orders 
                                           WHERE  orders.orderdate >= 
                                                  '2003-01-01' 
                                                  AND orders.orderdate <= 
                                                      '2003-12-31') 
                                     ) 
GROUP  BY offices.country
ORDER  BY nosell DESC;
