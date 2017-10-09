import {Component} from "@angular/core";
import {Car, CarService} from "../car.service";
import {Router} from "@angular/router";

@Component({
    selector: 'carmageddon-header',
    template: require('./header.html'),
    styles: [require('./header.scss')]
})
export class HeaderComponent {
    private car: Car;

    constructor(private carService: CarService,
                private router: Router) {

    }

    ngOnInit() {
        this.carService.getCar().subscribe(car => {
            if (this.car === undefined || this.car.mode !== car.mode) {
                if (car.mode === 0) {
                    this.router.navigate(['/off']);
                } else if (car.mode === 1) {
                    this.router.navigate(['/manual']);
                } else if (car.mode === 2) {
                    this.router.navigate(['/race']);
                }
            }
            this.car = car;

            console.log('Car ', this.car);
        });
    }

    setCarMode(mode: number) {
        this.carService.setCarMode(mode).subscribe();
    }

}