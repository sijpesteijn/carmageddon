import { Injectable } from '@angular/core';
import { Observable } from 'rxjs/Rx';
import { SettingsService } from '../settings.service';
import { Http } from '@angular/http';
import 'rxjs/add/operator/map';
import { Config } from '../app.config';

export interface CarObserver {
    condition_achieved: number;
    type: string;
    active: number;
    order: number;
    roi: Roi;
}

export interface Roi {
    x: number;
    y: number;
    width: number;
    height: number;
}

@Injectable()
export class ObserverService {

    constructor(private http: Http, private config: Config) {

    }

    getObserver(type: string): Observable<CarObserver> {
        return (this.http.get(this.config.get('observer.get').replace(':type', type), {} as any)
            .map(response => {
                const data = response.json();
                console.log('Ser: ', data);
                const observer: CarObserver = data;
                return observer;
            }));
    }

    saveObserver(type: string, observer: CarObserver): Observable<CarObserver> {
        return (this.http.post("http://carmageddon.dev/observer", observer)
            .map(response => {
                return response.json();
            }));
    }
}