
import {Injectable} from "@angular/core";
import {Config} from "./app.config";
import {Http} from "@angular/http";
import {Observable} from "rxjs/Observable";
import {Subject} from "rxjs/Subject";
import {BehaviorSubject} from "rxjs/BehaviorSubject";

export interface Car {
    throttle: number;
    angle: number;
    mode: number;
    enabled: number;
}

@Injectable()
export class CarService {
    private car: Subject<Car>;

    constructor(private http: Http, private config: Config) {
        this.car = new BehaviorSubject({throttle: 0, angle: 0, mode: 0, enabled: 0});
    }

    setEngine(throttle: number ) {
        return (this.http.post(this.config.get('car.engine').replace(':throttle', String(throttle)), {})
            .map(response => {
                console.log('Engine: ', response);
                // return response.json();
            }));

    }

    setSteer(angle: number ) {
        return (this.http.post(this.config.get('car.steer').replace(':angle', String(angle)), {})
            .map(response => {
                // console.log('Steer: ', response);
                // return response.json();
            }));

    }

    setCarMode(mode: number) {
        return (this.http.post(this.config.get('car.mode').replace(':mode', String(mode)), {})
            .map(response => {
                return response.json();
            }));
    }

    getCar(): Observable<Car> {
        return this.car;
    }

    updateCar(car: Car) {
        this.car.next(car);
    }
}