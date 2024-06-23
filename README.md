# Алгоритмизация перезарядки электросамокатов
По информации о расположении самокатов и данным исполнителей (длительность смены, максимальное количество перевозимых аккумуляторов) нужно построить им маршруты, чтобы перезарядить как можно больше самокатов

### Цель
- Реализовать генератор путей с заданными ограничениями на основе Google OR-Tools
- Подобрать оптимальные параметры
- Сравнить с существующими аналогами

## Ограничения и параметры
- Количество курьеров – 1, 4, 10. Максимальная загруженность – 50
- Ограничение времени на решение – 1, 3, 10, 30, 100, 300, 1000 секунд
- Начальная генерация:
  * PATH_CHEAPEST_ARC
  * SAVINGS
  * CHRISTOFIDES
  * PARALLEL_CHEAPEST_INSERTION
- Стратегии поиска (метаэвристики):
  * GUIDED_LOCAL_SEARCH
  * GREEDY_DESCENT
  * SIMULATED_ANNEALING
  * TABU_SEARCH
  * GENERIC_TABU_SEARCH

# Результаты

<div align="center"> 
 
## 1 курьер

 
<img src=https://github.com/mozhayka/scooters-ortools/blob/master/presentation/images/1-courier.svg>
 
<img src="https://github.com/mozhayka/scooters-ortools/blob/master/graphics/input1/couriers-1/result-4296_tl-100s_meta-GUIDED-LOCAL-SEARCH_start-PATH-CHEAPEST-ARC_opt-N.svg">

100 sec cheapest arc - 71 min

<img src="https://github.com/mozhayka/scooters-ortools/blob/master/graphics/input1/couriers-1/result-3866_tl-100s_meta-GUIDED-LOCAL-SEARCH_start-CHRISTOFIDES_opt-N.svg" background-color="#ffffff">

100 sec christofides - 64 min

## 4 курьера

<img src=https://github.com/mozhayka/scooters-ortools/blob/master/presentation/images/compare_starts.svg>
 
<img src=https://github.com/mozhayka/scooters-ortools/blob/master/presentation/images/compare_metas.svg background-color="#ffffff">

<img src="https://github.com/mozhayka/scooters-ortools/blob/master/graphics/input4/couriers-4/result-26531_tl-1000s_meta-GUIDED-LOCAL-SEARCH_start-CHRISTOFIDES_opt-Y.svg" background-color="#ffffff">

## 10 курьеров

<img src=https://github.com/mozhayka/scooters-ortools/blob/master/graphics/input5/couriers-10/result-74445_tl-1000s_meta-GUIDED-LOCAL-SEARCH_start-PATH-CHEAPEST-ARC_opt-Y.svg>
 
<img src=https://github.com/mozhayka/scooters-ortools/blob/master/graphics/input5/couriers-10/result-74518_tl-1000s_meta-GUIDED-LOCAL-SEARCH_start-CHRISTOFIDES_opt-Y.svg>

<img src=https://github.com/mozhayka/scooters-ortools/blob/master/graphics/input5/couriers-10/result-75096_tl-300s_meta-GUIDED-LOCAL-SEARCH_start-PATH-CHEAPEST-ARC_opt-Y.svg>

<img src=https://github.com/mozhayka/scooters-ortools/blob/master/graphics/input5/couriers-10/result-76627_tl-1000s_meta-TABU-SEARCH_start-PATH-CHEAPEST-ARC_opt-Y.svg>

</div>
