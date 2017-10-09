import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { NoLifelineError } from './no_lifeline/no_lifeline.component';
import { error_routes } from './error.routes';
import { RouterModule } from '@angular/router';
@NgModule({
    imports: [
        CommonModule,
        RouterModule.forChild(error_routes)
    ],
    declarations: [
        NoLifelineError
    ]
})
export class ErrorModule {
    public static routes = error_routes;
}