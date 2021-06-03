-- Consulta que muestra la cantidad de dinero abonado por los clientes que han adquirido el 1940 Ford Pickup Truck.
-- Realizo primero una consulta anidada para obtener el customernumber de los clientes que han adquirido el coche.
-- Hay que ordenarlo de menor a mayor cantidad.
SELECT customers.customernumber, 
       customers.customername, 
       Sum (payments.amount) AS gastado 
FROM   customers, 
       payments 
WHERE  customers.customernumber = payments.customernumber 
       AND customers.customernumber IN (SELECT customers.customernumber 
                                        FROM   customers, 
                                               orders, 
                                               orderdetails, 
                                               products 
                                        WHERE  products.productcode = 
                                               orderdetails.productcode 
                                               AND 
               orderdetails.ordernumber = orders.ordernumber 
                                               AND orders.customernumber = 
                                                   customers.customernumber 
                                               AND 
               products.productname = '1940 Ford Pickup Truck') 
GROUP  BY customers.customernumber, 
          customers.customername 
ORDER  BY gastado DESC;
