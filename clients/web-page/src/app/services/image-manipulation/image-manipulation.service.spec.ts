import { TestBed } from '@angular/core/testing';
import { provideHttpClient } from '@angular/common/http';
import { beforeEach, describe, expect, it } from 'vitest';

import { ImageManipulationService } from './image-manipulation.service';

describe('ImageManipulationService', () => {
  beforeEach(() => TestBed.configureTestingModule({
    providers: [provideHttpClient()]
  }));

  it('should be created', () => {
    const service = TestBed.inject(ImageManipulationService);
    expect(service).toBeTruthy();
  });
});
