select *
from fifo 
where
	vrsta='P' and
	cena < 59.5 and
	papir_id=' KUC'
order by	
	cas;