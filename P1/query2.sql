-- Tiempo medio transcurrido entre que se realiza un pedido y se envía. Para ello, realizamos el AVG de la resta de ambos datos.
SELECT products.productline, 
       Avg(orders.shippeddate - orders.orderdate) 
FROM   products, 
       orderdetails, 
       orders 
WHERE  orders.ordernumber = orderdetails.ordernumber 
       AND orderdetails.productcode = products.productcode 
GROUP  BY products.productline; 
