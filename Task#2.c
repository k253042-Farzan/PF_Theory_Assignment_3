#include <stdio.h>

void calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets){
    // Base case: if fuel finished or journey completed
    if (fuel <= 0) {
        printf("Planet %d: Fuel Remaining = 0\n", planet);
        printf("Mission Failed! Spacecraft ran out of fuel.\n");
        return;
    }

    if (planet > totalPlanets) {
        printf("Mission Successful! Spacecraft completed the journey.\n");
        return;
    }

    // Apply fuel consumption at each planet
    fuel -= consumption;

    // If fuel finishes after consumption
    if (fuel <= 0) {
        printf("Planet %d: Fuel Remaining = 0\n", planet);
        printf("Mission Failed! Spacecraft ran out of fuel.\n");
        return;
    }

    // Apply gravitational recharge
    fuel += recharge;

    // Apply solar recharge after every 4th planet
    if (planet % 4 == 0) {
        fuel += solarBonus;
    }

    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);

    // Recursive call for next planet
    calculateFuel(fuel, consumption, recharge, solarBonus, planet + 1, totalPlanets);
}

int main()
{
    int fuel, consumption, recharge, solarBonus, totalPlanets;

    printf("Enter initial fuel: ");
    scanf("%d", &fuel);

    printf("Enter fuel consumed per planet: ");
    scanf("%d", &consumption);

    printf("Enter gravitational recharge amount: ");
    scanf("%d", &recharge);

    printf("Enter solar bonus (every 4th planet): ");
    scanf("%d", &solarBonus);

    printf("Enter total number of planets: ");
    scanf("%d", &totalPlanets);

    calculateFuel(fuel, consumption, recharge, solarBonus, 1, totalPlanets);

    return 0;
}