
import {Injectable} from "@angular/core";
import {Config} from "./app.config";
import {Http} from "@angular/http";
import {Observable} from "rxjs/Observable";
import {Subject} from "rxjs/Subject";
import {BehaviorSubject} from "rxjs/BehaviorSubject";

export interface Dimension {
    name: string;
    width: number;
    height: number;
}
export interface CameraSettings {
    dimension?: Dimension;
    captureDelay?: number;
    previewDelay?: number;
}

@Injectable()
export class CameraService {
    private cameraSetting: Subject<CameraSettings>;

    constructor(private http: Http, private config: Config) {
        this.cameraSetting = new BehaviorSubject(null);
        this.http.get(this.config.get('camera.settings'))
            .subscribe(response => {
                this.cameraSetting.next(response.json());
            });

    }

    getCameraInfo(): Observable<CameraSettings> {
        return this.cameraSetting;
    }

    updateCameraInfo(cameraSettings: CameraSettings): Observable<CameraSettings> {
        const json = JSON.stringify(cameraSettings);
        return this.http.post(this.config.get('camera.settings'), json).map(response => {
            const cameraSettings = response.json();
            this.cameraSetting.next(cameraSettings);
            return cameraSettings;
        });
    }

    getDimensions(): Observable<Dimension[]> {
        return this.http.get(this.config.get('camera.dimensions')).map(response => {
            return response.json();
        });
    }
}