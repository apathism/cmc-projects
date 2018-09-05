from math import ceil
from config import WEEKS_IN_MONTH, MONTHS_IN_YEAR


class TownInfectionState():
    def __init__(self, town):
        self.town = town
        self.ill_population = (0, 0, 0, 0)
        self.vaccinated_population = (town.population, 0, 0, 0)

    def get_total_ill_population(self):
        return sum(self.ill_population)

    def get_total_vaccinated_population(self):
        return sum(self.vaccinated_population)

    def copy(self):
        new_state = TownInfectionState(self.town)
        new_state.ill_population = self.ill_population
        new_state.vaccinated_population = self.vaccinated_population
        return new_state

    def get_next_ill_state(self, month):
        new_state = self.copy()
        make_ill = ceil(0.05 * new_state.vaccinated_population[0])
        make_vaccinated = ceil(0.05 * new_state.ill_population[0])
        new_state.vaccinated_population = (new_state.vaccinated_population[0] - make_ill,
                                           new_state.vaccinated_population[1],
                                           new_state.vaccinated_population[2],
                                           new_state.vaccinated_population[3] + make_vaccinated)
        new_state.ill_population = (new_state.ill_population[0] + make_ill - make_vaccinated,
                                    new_state.ill_population[1],
                                    new_state.ill_population[2],
                                    new_state.ill_population[3])
        return new_state


class Town():
    roads = []

    def __init__(self, name, population, x=0.0, y=0.0, town_infection_state=None):
        self.name = name
        self.population = population
        self.x = x
        self.y = y
        self.infection_state = town_infection_state if town_infection_state else TownInfectionState(self)

    def get_nearby_towns(self):
        return list([(road.get_neighbour(self), road.traffic) for road in self.roads])

    def reset_town_state(self):
        self.infection_state = TownInfectionState(self)


class Road():
    def __init__(self, first_town, second_town, traffic=0.5):
        self.first_town = first_town
        self.second_town = second_town
        self.traffic = traffic

    def get_neighbour(self, town):
        if town is self.first_town:
            return self.second_town
        if town is self.second_town:
            return self.first_town
        return None


class ModelState():
    current_step = 0
    towns = []
    roads = []

    def __init__(self, start_week=1, start_month=8, max_step=24, current_money=0):
        self.week = start_week
        self.month = start_month
        self.current_money = current_money
        self.max_step = max_step

    def reset_model(self):
        self.week -= self.current_step
        if self.week <= 0:
            self.month = (self.month + (self.week - 1) // WEEKS_IN_MONTH - 1) % MONTHS_IN_YEAR + 1
            self.week = (self.week - 1) % WEEKS_IN_MONTH + 1
        self.current_step = 0
        for town in self.towns:
            town.reset_town_state()

    def make_step(self):
        states = [(town, town.infection_state.get_next_ill_state(self.month)) for town in self.towns]
        for town, state in states:
            town.infection_state = state