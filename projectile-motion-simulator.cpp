#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

const float PI = 3.14159265f;

struct Vector2D {
    float x, y;
    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}
};

struct ProjectileData {
    float initialVelocity;
    float angle; // in degrees
    float gravity;
    bool airResistance;
    float dragCoefficient;
    float mass;
    
    ProjectileData() : initialVelocity(50.0f), angle(45.0f), gravity(9.8f), 
                       airResistance(false), dragCoefficient(0.47f), mass(1.0f) {}
};

class ProjectileSimulator {
private:
    ProjectileData data;
    std::vector<Vector2D> trajectoryPoints;
    
public:
    ProjectileSimulator(const ProjectileData& data) : data(data) {}
    
    void calculateTrajectory() {
        trajectoryPoints.clear();
        
        if (!data.airResistance) {
            calculateAnalytical();
        } else {
            calculateNumerical();
        }
    }
    
    void calculateAnalytical() {
        float angleRad = data.angle * PI / 180.0f;
        float vx = data.initialVelocity * cos(angleRad);
        float vy = data.initialVelocity * sin(angleRad);
        
        float totalTime = 2.0f * vy / data.gravity;
        float dt = 0.02f;
        
        for (float t = 0; t <= totalTime; t += dt) {
            float x = vx * t;
            float y = vy * t - 0.5f * data.gravity * t * t;
            
            if (y < 0) break;
            trajectoryPoints.push_back(Vector2D(x, y));
        }
    }
    
    void calculateNumerical() {
        float angleRad = data.angle * PI / 180.0f;
        float vx = data.initialVelocity * cos(angleRad);
        float vy = data.initialVelocity * sin(angleRad);
        
        float x = 0, y = 0;
        float dt = 0.01f;
        
        const float AIR_DENSITY = 1.225f;
        const float CROSS_SECTION_AREA = 0.01f;
        
        while (y >= 0) {
            trajectoryPoints.push_back(Vector2D(x, y));
            
            float speed = sqrt(vx * vx + vy * vy);
            float dragForce = 0.5f * AIR_DENSITY * data.dragCoefficient * 
                             CROSS_SECTION_AREA * speed * speed;
            
            float dragAccelX = 0, dragAccelY = 0;
            if (speed > 0.001f) {
                dragAccelX = -(dragForce / data.mass) * (vx / speed);
                dragAccelY = -(dragForce / data.mass) * (vy / speed);
            }
            
            vx += dragAccelX * dt;
            vy += (dragAccelY - data.gravity) * dt;
            
            x += vx * dt;
            y += vy * dt;
            
            if (trajectoryPoints.size() > 10000) break;
        }
    }
    
    float getMaxHeight() const {
        float maxH = 0;
        for (const auto& point : trajectoryPoints) {
            if (point.y > maxH) maxH = point.y;
        }
        return maxH;
    }
    
    float getRange() const {
        if (trajectoryPoints.empty()) return 0;
        return trajectoryPoints.back().x;
    }
    
    float getFlightTime() const {
        return trajectoryPoints.size() * (data.airResistance ? 0.01f : 0.02f);
    }
    
    void printResults() const {
        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout << "║   PROJECTILE MOTION SIMULATOR          ║\n";
        std::cout << "╚════════════════════════════════════════╝\n\n";
        
        std::cout << "📊 INPUT PARAMETERS:\n";
        std::cout << "├─ Initial Velocity: " << data.initialVelocity << " m/s\n";
        std::cout << "├─ Launch Angle: " << data.angle << "°\n";
        std::cout << "├─ Gravity: " << data.gravity << " m/s²\n";
        std::cout << "└─ Air Resistance: " << (data.airResistance ? "ON" : "OFF") << "\n\n";
        
        std::cout << "📈 RESULTS:\n";
        std::cout << "├─ Maximum Height: " << std::fixed << std::setprecision(2) 
                  << getMaxHeight() << " m\n";
        std::cout << "├─ Range: " << getRange() << " m\n";
        std::cout << "├─ Flight Time: " << getFlightTime() << " s\n";
        
        if (!data.airResistance) {
            float angleRad = data.angle * PI / 180.0f;
            float impactVelocity = data.initialVelocity;
            std::cout << "└─ Impact Velocity: " << impactVelocity << " m/s\n\n";
        } else {
            std::cout << "└─ (Air resistance affects impact velocity)\n\n";
        }
    }
    
    void visualizeTrajectory() const {
        std::cout << "🎯 TRAJECTORY VISUALIZATION:\n\n";
        
        const int WIDTH = 80;
        const int HEIGHT = 25;
        
        char canvas[HEIGHT][WIDTH];
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                canvas[i][j] = ' ';
            }
        }
        
        // Draw ground
        for (int j = 0; j < WIDTH; j++) {
            canvas[HEIGHT-1][j] = '─';
        }
        
        // Find max values for scaling
        float maxX = getRange();
        float maxY = getMaxHeight();
        
        // Plot trajectory
        for (const auto& point : trajectoryPoints) {
            int x = (int)((point.x / maxX) * (WIDTH - 1));
            int y = HEIGHT - 2 - (int)((point.y / maxY) * (HEIGHT - 2));
            
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT - 1) {
                canvas[y][x] = '*';
            }
        }
        
        // Mark starting point
        canvas[HEIGHT-2][0] = 'S';
        
        // Mark landing point
        int endX = (int)((getRange() / maxX) * (WIDTH - 1));
        if (endX < WIDTH - 1) {
            canvas[HEIGHT-2][endX] = 'L';
        }
        
        // Print canvas
        std::cout << "  ┌" << std::string(WIDTH, '─') << "┐\n";
        for (int i = 0; i < HEIGHT; i++) {
            std::cout << "  │";
            for (int j = 0; j < WIDTH; j++) {
                std::cout << canvas[i][j];
            }
            std::cout << "│\n";
        }
        std::cout << "  └" << std::string(WIDTH, '─') << "┘\n";
        std::cout << "  S = Start, L = Landing, * = Trajectory\n\n";
        
        std::cout << "  Scale: " << std::fixed << std::setprecision(1) 
                  << maxX << " m horizontal, " << maxY << " m vertical\n\n";
    }
    
    void showTrajectoryData() const {
        std::cout << "📋 TRAJECTORY DATA (sample points):\n";
        std::cout << std::string(50, '─') << "\n";
        std::cout << std::setw(10) << "Time(s)" << std::setw(15) << "X(m)" 
                  << std::setw(15) << "Y(m)" << "\n";
        std::cout << std::string(50, '─') << "\n";
        
        float dt = data.airResistance ? 0.01f : 0.02f;
        size_t step = trajectoryPoints.size() / 10;
        if (step == 0) step = 1;
        
        for (size_t i = 0; i < trajectoryPoints.size(); i += step) {
            float time = i * dt;
            std::cout << std::fixed << std::setprecision(2)
                      << std::setw(10) << time 
                      << std::setw(15) << trajectoryPoints[i].x
                      << std::setw(15) << trajectoryPoints[i].y << "\n";
        }
        std::cout << std::string(50, '─') << "\n\n";
    }
};

void displayMenu() {
    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║          MENU OPTIONS                  ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
    std::cout << "1. Run new simulation\n";
    std::cout << "2. Compare angles (optimize for range)\n";
    std::cout << "3. Compare with/without air resistance\n";
    std::cout << "4. Test different planets\n";
    std::cout << "5. Exit\n";
    std::cout << "\nEnter choice: ";
}

void runSimulation() {
    ProjectileData data;
    
    std::cout << "\n═══ NEW SIMULATION ═══\n";
    std::cout << "Enter initial velocity (m/s): ";
    std::cin >> data.initialVelocity;
    
    std::cout << "Enter launch angle (0-90 degrees): ";
    std::cin >> data.angle;
    
    std::cout << "Include air resistance? (1=Yes, 0=No): ";
    int airChoice;
    std::cin >> airChoice;
    data.airResistance = (airChoice == 1);
    
    ProjectileSimulator simulator(data);
    simulator.calculateTrajectory();
    simulator.printResults();
    simulator.visualizeTrajectory();
    
    std::cout << "Show detailed trajectory data? (1=Yes, 0=No): ";
    int showData;
    std::cin >> showData;
    if (showData == 1) {
        simulator.showTrajectoryData();
    }
}

void compareAngles() {
    std::cout << "\n═══ ANGLE OPTIMIZATION ═══\n";
    std::cout << "Enter velocity (m/s): ";
    float velocity;
    std::cin >> velocity;
    
    std::cout << "\nComparing angles from 15° to 75° (Earth gravity, no air resistance):\n\n";
    std::cout << std::string(60, '─') << "\n";
    std::cout << std::setw(15) << "Angle" << std::setw(20) << "Range(m)" 
              << std::setw(20) << "Max Height(m)" << "\n";
    std::cout << std::string(60, '─') << "\n";
    
    float bestAngle = 0;
    float bestRange = 0;
    
    for (int angle = 15; angle <= 75; angle += 5) {
        ProjectileData data;
        data.initialVelocity = velocity;
        data.angle = angle;
        data.airResistance = false;
        
        ProjectileSimulator sim(data);
        sim.calculateTrajectory();
        
        float range = sim.getRange();
        if (range > bestRange) {
            bestRange = range;
            bestAngle = angle;
        }
        
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(15) << angle << "°"
                  << std::setw(20) << range
                  << std::setw(20) << sim.getMaxHeight() << "\n";
    }
    
    std::cout << std::string(60, '─') << "\n";
    std::cout << "✨ Optimal angle: " << bestAngle << "° with range: " 
              << bestRange << " m\n\n";
}

void compareAirResistance() {
    std::cout << "\n═══ AIR RESISTANCE COMPARISON ═══\n";
    std::cout << "Enter velocity (m/s): ";
    float velocity;
    std::cin >> velocity;
    
    std::cout << "Enter angle (degrees): ";
    float angle;
    std::cin >> angle;
    
    // Without air resistance
    ProjectileData data1;
    data1.initialVelocity = velocity;
    data1.angle = angle;
    data1.airResistance = false;
    
    ProjectileSimulator sim1(data1);
    sim1.calculateTrajectory();
    
    // With air resistance
    ProjectileData data2;
    data2.initialVelocity = velocity;
    data2.angle = angle;
    data2.airResistance = true;
    
    ProjectileSimulator sim2(data2);
    sim2.calculateTrajectory();
    
    std::cout << "\n" << std::string(70, '─') << "\n";
    std::cout << std::setw(30) << " " << std::setw(20) << "Without Air" 
              << std::setw(20) << "With Air" << "\n";
    std::cout << std::string(70, '─') << "\n";
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(30) << "Range (m):" 
              << std::setw(20) << sim1.getRange()
              << std::setw(20) << sim2.getRange() << "\n";
    
    std::cout << std::setw(30) << "Max Height (m):" 
              << std::setw(20) << sim1.getMaxHeight()
              << std::setw(20) << sim2.getMaxHeight() << "\n";
    
    std::cout << std::setw(30) << "Flight Time (s):" 
              << std::setw(20) << sim1.getFlightTime()
              << std::setw(20) << sim2.getFlightTime() << "\n";
    
    std::cout << std::string(70, '─') << "\n";
    
    float rangeLoss = ((sim1.getRange() - sim2.getRange()) / sim1.getRange()) * 100;
    std::cout << "\n📉 Range reduction due to air resistance: " 
              << rangeLoss << "%\n\n";
}

void testPlanets() {
    std::cout << "\n═══ PLANETARY COMPARISON ═══\n";
    std::cout << "Enter velocity (m/s): ";
    float velocity;
    std::cin >> velocity;
    
    std::cout << "Enter angle (degrees): ";
    float angle;
    std::cin >> angle;
    
    struct Planet {
        std::string name;
        float gravity;
    };
    
    Planet planets[] = {
        {"Earth", 9.8f},
        {"Moon", 1.62f},
        {"Mars", 3.71f},
        {"Jupiter", 24.79f},
        {"Venus", 8.87f}
    };
    
    std::cout << "\n" << std::string(75, '─') << "\n";
    std::cout << std::setw(15) << "Planet" 
              << std::setw(15) << "Gravity(m/s²)"
              << std::setw(20) << "Range(m)" 
              << std::setw(20) << "Max Height(m)" << "\n";
    std::cout << std::string(75, '─') << "\n";
    
    for (const auto& planet : planets) {
        ProjectileData data;
        data.initialVelocity = velocity;
        data.angle = angle;
        data.gravity = planet.gravity;
        data.airResistance = false;
        
        ProjectileSimulator sim(data);
        sim.calculateTrajectory();
        
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(15) << planet.name
                  << std::setw(15) << planet.gravity
                  << std::setw(20) << sim.getRange()
                  << std::setw(20) << sim.getMaxHeight() << "\n";
    }
    
    std::cout << std::string(75, '─') << "\n\n";
}

int main() {
    std::cout << R"(
    ╔═══════════════════════════════════════════════════════╗
    ║                                                       ║
    ║     PROJECTILE MOTION SIMULATOR                       ║
    ║     Physics Simulation & Analysis Tool                ║
    ║                                                       ║
    ╚═══════════════════════════════════════════════════════╝
    )" << "\n";
    
    int choice;
    do {
        displayMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                runSimulation();
                break;
            case 2:
                compareAngles();
                break;
            case 3:
                compareAirResistance();
                break;
            case 4:
                testPlanets();
                break;
            case 5:
                std::cout << "\n👋 Thanks for using the simulator! Goodbye!\n\n";
                break;
            default:
                std::cout << "\n❌ Invalid choice. Try again.\n";
        }
        
        if (choice != 5) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
        
    } while (choice != 5);
    
    return 0;
}
