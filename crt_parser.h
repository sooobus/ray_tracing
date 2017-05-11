#include <map>
#include <string>
#include "scene.h"
#include "geom.h"


class Material{
public:
    Material(RGBColor color_, double alpha_, double reflect_, double refract_) : color(color_), alpha(alpha_), reflect(reflect_)
, refract(refract_){};
    Material(){
      ;
    };

    RGBColor color;
    double alpha;
    double reflect;
    double refract;
};


class LoadFromRt {
public:
    Scene* load(string filePath) {
        cout << "LOADER " << filePath << endl;
        cout.flush();
        std::ifstream fileStream(filePath);

        if (!fileStream.good()) {
            cout << "Иди нахуй со своим файлом " << strerror(errno) << endl;
            throw "file opening error";
        }

        cout << "Не пошёл?" << endl;

        /*
        string line;
        for(int i = 0; i < 100; i++)
            {
              coutline(fileStream);
            }

            exit(0);
           */

        std::string currentToken;
        std::map<std::string, Material> materials;
        vector<GeomObj*> objects;
        vector<PointLightSource*> lights;
        vector<ThreeDVector> screen_params;

        while (true) {
            fileStream >> currentToken;
            cout << currentToken << endl;
            if (currentToken == "viewport") {
                screen_params = readViewport(fileStream);
                cout << "VIEWPORT" << endl;
                cout.flush();
            }
            else if (currentToken == "materials") {
                cout << "YEP" << endl;
                cout.flush();
                materials = readMaterials(fileStream);
                cout << "MATERIALS" << endl;
            }
            else if (currentToken == "lights") {
                lights = readLights(fileStream);
                cout << "LIGHTS" << endl;
            }
            else { // geometry
                objects = readGeometry(fileStream, materials);
                cout << "GEOM" << endl;
                break;
            }
        }

        fileStream.close();

        auto ul = screen_params[1];
        auto ur = screen_params[3];
        auto dl = screen_params[2];
        //auto dr = ur + (ul - dl);
        auto scene_ptr = new Scene(objects, lights, ul, ur, dl, screen_params[0], 800, 600);
        return scene_ptr;
    }

    void coutline(std::ifstream &fs){
        string line;
        fs >> line;
        cout << line << "!\n";
    }

    ThreeDVector readPoint(std::ifstream &fileStream) {
        double x, y, z;
        fileStream >> x >> y >> z;
        return ThreeDVector(x, y, z);
    }

    RGBColor readColor(std::ifstream &fileStream) {
        double x, y, z;
        fileStream >> x >> y >> z;
        return RGBColor(x, y, z);
    }

    vector<ThreeDVector> readViewport(std::ifstream &fileStream) {
        std::string currentToken;
        cout << "read viewport" << endl;
        vector<ThreeDVector> screen_params;
        ThreeDVector origin, topLeftCorner, bottomLeftCorner, topRightCorner;
        for (int i = 0; i < 4; ++i) {
            fileStream >> currentToken;
            cout << currentToken << endl;
            ThreeDVector currentPoint = readPoint(fileStream);
            if (currentToken == "origin") {
                origin = currentPoint;
            }
            else if (currentToken == "topleft") {
                topLeftCorner = currentPoint;
            }
            else if (currentToken == "bottomleft") {
                bottomLeftCorner = currentPoint;
            }
            else { // topright
                topRightCorner = currentPoint;
            }
        }
        screen_params.push_back(origin);
        screen_params.push_back(topLeftCorner);
        screen_params.push_back(bottomLeftCorner);
        screen_params.push_back(topRightCorner);

        fileStream >> currentToken; // endviewport
        cout << "MUST: endviewport>>" << currentToken << endl;

        return screen_params;
    }

    std::map<std::string, Material> readMaterials(std::ifstream &fileStream) {
        std::string currentToken;
        std::map<std::string, Material> materials;

        while (true) {
            fileStream >> currentToken; // entry or endmaterials

            cout << "MUST: entry>> " << currentToken << endl;

            if (currentToken == "endmaterials") {
                break;
            }

            std::string name;
            RGBColor color(0, 0, 0);
            double reflect = 0, refract = 0, alpha=0;

            for (int i = 0; i < 6; ++i) {
                fileStream >> currentToken;
                if (currentToken == "name") {
                    fileStream >> name;
                }
                else if (currentToken == "color") {
                    color = readColor(fileStream);
                }
                else if (currentToken == "reflect") {
                    fileStream >> reflect;
                }
                else if (currentToken == "alpha") {
                    fileStream >> alpha;
                }
                else if (currentToken == "refract") {
                    fileStream >> refract;
                }
                else {
                    break; // endentry
                }
            }

            Material m(color, alpha, reflect, refract);
            materials[name] = m;
        }

        return materials;
    }

    vector<PointLightSource*> readLights(std::ifstream &fileStream) {
        vector<PointLightSource *> ans;
        std::string currentToken;
        double referencePower = 1, referenceDistance = 1;

        while (true) {
            fileStream >> currentToken; // point or endlights

            if (currentToken == "reference") {
                for (int i = 0; i < 2; ++i) {
                    fileStream >> currentToken;
                    if (currentToken == "power") {
                        fileStream >> referencePower;
                    }
                    else if (currentToken == "distance") {
                        fileStream >> referenceDistance;
                    }
                }
                fileStream >> currentToken; // endreference
                continue;
            }
            else if (currentToken == "endlights") {
                break;
            }

            ThreeDVector point(0, 0, 0);
            double power = 1;

            for (int i = 0; i < 2; ++i) {
                fileStream >> currentToken;
                if (currentToken == "coords") {
                    point = readPoint(fileStream);
                }
                else { // power
                    fileStream >> power;
                }
            }
            fileStream >> currentToken; // endpoint

            ans.push_back(new PointLightSource(point, power / referencePower * referenceDistance * referenceDistance));
        }
        return ans;
    }

    vector<GeomObj*> readGeometry(std::ifstream &fileStream, std::map<std::string, Material> &materials) {
        std::string currentToken;

        vector<GeomObj*> objects;

        while (true) {
            fileStream >> currentToken; // point or endlights

            if (currentToken == "sphere") {
                objects.push_back(readSphere(fileStream, materials));
            }
            else if (currentToken == "triangle") {
                objects.push_back(readTriangle(fileStream, materials));
            }
            else if (currentToken == "quadrangle") {
                objects.push_back(readQuadrangle(fileStream, materials));
            } else { // endgeometry
                break;
            }
        }

        return objects;
    }

    Sphere* readSphere(std::ifstream &fileStream, std::map<std::string, Material> &materials) {
        std::string currentToken;
        ThreeDVector center;
        double radius = 1;
        std::string materialName;

        for (int i = 0; i < 3; ++i) {
            fileStream >> currentToken;
            if (currentToken == "vertex") {
                center = readPoint(fileStream);
            }
            else if (currentToken == "radius") {
                fileStream >> radius;
            }
            else { // material
                fileStream >> materialName;
            }
        }
        fileStream >> currentToken; // endsphere

        return new Sphere(center, radius, materials[materialName].color);
    }

    Triangle* readTriangle(std::ifstream &fileStream, std::map<std::string, Material> &materials) {
        std::string currentToken;
        std::vector<ThreeDVector> vertices;
        std::string materialName;

        for (int i = 0; i < 4; ++i) {
            fileStream >> currentToken;
            if (currentToken == "vertex") {
                vertices.push_back(readPoint(fileStream));
            }
            else { // material
                fileStream >> materialName;
            }
        }
        fileStream >> currentToken; // endtriangle

        return new Triangle(vertices[0], vertices[1], vertices[2], materials[materialName].color);
    }

    Quadrilateral* readQuadrangle(std::ifstream &fileStream, std::map<std::string, Material> &materials) {
        std::string currentToken;
        std::vector<ThreeDVector> vertices;
        std::string materialName;

        for (int i = 0; i < 5; ++i) {
            fileStream >> currentToken;
            if (currentToken == "vertex") {
                vertices.push_back(readPoint(fileStream));
            }
            else { // material
                fileStream >> materialName;
            }
        }
        fileStream >> currentToken; // endquadrangle

        return new Quadrilateral(vertices[0], vertices[1], vertices[2], vertices[3], materials[materialName].color);
    }
};
