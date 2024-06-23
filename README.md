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

### 1 курьер
<p align="center"> 
 <img src=https://github.com/mozhayka/scooters-ortools/blob/master/presentation/images/1-courier.svg>
</p>

<div align="center">
  <img src="https://github.com/mozhayka/scooters-ortools/blob/master/graphics/input1/couriers-1/result-4296_tl-100s_meta-GUIDED-LOCAL-SEARCH_start-PATH-CHEAPEST-ARC_opt-N.svg" background-color="#ffffff">
</div>

