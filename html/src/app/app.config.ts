
import { Inject, Injectable } from '@angular/core';
import { Http } from '@angular/http';
@Injectable()
export class Config {
    private config: any;

    constructor(private http: Http) {

    }

    public load(jsonFile: string) {
        return new Promise((resolve, reject) => {
            this.http.get(jsonFile).map(res => res.json()).subscribe(data => {
                this.config = data;
                resolve(true);
            });
        });
    }

    public get(key: string): string {
        const keys = key.split('.');
        let result = this.config;
        keys.forEach((k) => result = result[k]);
        return result;
    }
}